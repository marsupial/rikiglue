extern "C"
{
	#include "putty/putty.h"
}

static WSAEVENT sEvent;

extern "C"
{

int get_userpass_input(prompts_t *p, unsigned char *in, int inlen, const Config *cfg)
{
	strcpy(p->prompts[0]->result, cfg->password);
	return 1;
}

int verify_ssh_host_key(void *frontend, char *host, int port, char *keytype,
                        char *keystr, char *fingerprint,
                        void (*callback)(void *ctx, int result), void *ctx)
{
	return 1;
}

int askalg(void *frontend, const char *algtype, const char *algname,
	   void (*callback)(void *ctx, int result), void *ctx)
{
	return 1;
}

int agent_exists()
{
	return ( FALSE );
}

int agent_query(void *in, int inlen, void **out, int *outlen,
		void (*callback)(void *, void *, int), void *callback_ctx)
{
	*out = NULL;
	*outlen = 0;
	return ( 1 );
}

int from_backend(void *frontend, int is_stderr, const char *data, int len)
{
    return 0;
}

int from_backend_untrusted(void *frontend, const char *data, int len)
{
    return 0;
}

char *get_ttymode(void *frontend, const char *mode)
{
    return dupstr("^H");
}

int eventLoop( HANDLE    otherEvent )
{
	int n, nhandles, nallhandles, netindex, otherindex;
	HANDLE *handles;
	SOCKET *sklist;
	int skcount;

	handles = handle_get_events(&nhandles);
	handles = sresize(handles, nhandles+2, HANDLE);
	nallhandles = nhandles;

	if ( sEvent != INVALID_HANDLE_VALUE )
		handles[netindex = nallhandles++] = sEvent;
	else
		netindex = -1;

	if ( otherEvent != INVALID_HANDLE_VALUE )
		handles[otherindex = nallhandles++] = otherEvent;
    else
		otherindex = -1;

    n = WaitForMultipleObjects(nallhandles, handles, FALSE, INFINITE);

    if ( (unsigned)(n - WAIT_OBJECT_0) < (unsigned)nhandles )
	{
		handle_got_event(handles[n - WAIT_OBJECT_0]);
    }
	else if ( netindex >= 0 && n == WAIT_OBJECT_0 + netindex )
	{
		WSANETWORKEVENTS things;
		SOCKET socket;
		extern SOCKET first_socket(int *), next_socket(int *);
		extern int select_result(WPARAM, LPARAM);
		int i, socketstate;

		/*
		* We must not call select_result() for any socket
		* until we have finished enumerating within the
		* tree. This is because select_result() may close
		* the socket and modify the tree.
		*/
		/* Count the active sockets. */
		i = 0;
		for ( socket = first_socket(&socketstate);
		      socket != INVALID_SOCKET; socket = next_socket(&socketstate) )
			i++;

		/* Expand the buffer if necessary. */
		sklist = snewn(i, SOCKET);

		/* Retrieve the sockets into sklist. */
		skcount = 0;
		for ( socket = first_socket(&socketstate);
		      socket != INVALID_SOCKET;
		      socket = next_socket(&socketstate) )
			sklist[skcount++] = socket;

		/* Now we're done enumerating; go through the list. */
		for (i = 0; i < skcount; i++) {
			WPARAM wp;
			socket = sklist[i];
			wp = (WPARAM) socket;
			if ( !p_WSAEnumNetworkEvents(socket, NULL, &things) )
			{
				static const struct { int bit, mask; } eventtypes[] =
				{
					{FD_CONNECT_BIT, FD_CONNECT},
					{FD_READ_BIT, FD_READ},
					{FD_CLOSE_BIT, FD_CLOSE},
					{FD_OOB_BIT, FD_OOB},
					{FD_WRITE_BIT, FD_WRITE},
					{FD_ACCEPT_BIT, FD_ACCEPT},
				};
				int e;

				noise_ultralight(socket);
				noise_ultralight(things.lNetworkEvents);

				for ( e = 0; e < lenof(eventtypes); e++ )
					if ( things.lNetworkEvents & eventtypes[e].mask )
					{
						LPARAM lp;
						int err = things.iErrorCode[eventtypes[e].bit];
						lp = WSAMAKESELECTREPLY(eventtypes[e].mask, err);
						select_result(wp, lp);
					}
			}
		}
		sfree(sklist);
	}
	
	sfree(handles);

    if ( otherindex >= 0 && n == WAIT_OBJECT_0 + otherindex )
		return ( 1 );

	return ( 0 );
}

char *do_select( SOCKET    skt,
                 int       startup )
{
	int events = startup ? (FD_CONNECT | FD_READ | FD_WRITE | FD_OOB | FD_CLOSE | FD_ACCEPT) : 0;
	int err = p_WSAEventSelect(skt, sEvent, events);
	if ( err == SOCKET_ERROR)
	{
		switch ( p_WSAGetLastError() )
		{
			case WSAENETDOWN:
				return "Network is down";
			default:
				return "WSAEventSelect(): unknown error";
		}
	}
	return ( NULL );
}

static void
init( Config    &cfg )
{
	memset(&cfg, 0, sizeof(cfg));
	cfg.port = 22;
	cfg.protocol = PROT_SSH;
	cfg.close_on_exit = 2;
	cfg.warn_on_close = 0;
	cfg.tcp_nodelay = 1;
	cfg.proxy_dns = 2;
	cfg.proxy_port = 80;
	cfg.sshprot = 2;
	cfg.serspeed = 9600;
	cfg.serstopbits = 2;
	cfg.serflow = 1;
	cfg.bksp_is_delete = 1;
	cfg.remote_qtitle_action = 1;
	cfg.logtype = LGTYP_NONE;
	cfg.ssh_kexlist[0] = 3;
	cfg.ssh_kexlist[1] = 2;
	cfg.ssh_kexlist[2] = 1;
	cfg.ssh_kexlist[3] = 0;
	cfg.ssh_cipherlist[0] = 3;
	cfg.ssh_cipherlist[1] = 2;
	cfg.ssh_cipherlist[2] = 1;
	cfg.ssh_cipherlist[3] = 0;
	cfg.ssh_cipherlist[4] = 5;
	cfg.ssh_cipherlist[5] = 4;
	cfg.try_ki_auth = 1;
	cfg.ssh_subsys = 0;//1;
	cfg.ssh_subsys2 = 0;
	strcpy(cfg.serline, "COM1");
	cfg.serdatabits = 8;
	cfg.telnet_newline = 1;
	cfg.alt_f4 = 1;
	cfg.localecho = 2;
	cfg.localedit = 2;
	cfg.scroll_on_disp = 1;
	cfg.erase_to_scrollback = 1;
	strcpy(cfg.answerback, "PuTTY");
	cfg.vtmode = 4;
	cfg.sshbug_ignore1 = 2;
	cfg.sshbug_plainpw1 = 2;
	cfg.sshbug_rsa1 = 2;
	cfg.sshbug_hmac2 = 2;
	cfg.sshbug_derivekey2 = 2;
	cfg.sshbug_rsapad2 = 2;
	cfg.sshbug_pksessid2 = 2;
	cfg.sshbug_rekey2 = 2;
	cfg.stamp_utmp = 1;
	cfg.login_shell = 1;
}

int
putty_main( int           argc,
            const char    **argv )
{
	if ( argc < 4 )
	{
		printf("usage: <user> <host> <password> <cmd>\n");
		return ( -1 );
	}
	Config cfg;
	init(cfg);

	strcpy(cfg.username, argv[1]);
	strcpy(cfg.host, argv[2]);
	strcpy(cfg.password, argv[3]);
	strcpy(cfg.remote_cmd, argv[4]);
	// "touch ssh.txt; mv ~/html/index2.html ~/html/index.ssh.html;");

	sk_init();

	void *backhandle;
	const char *error;
	char *realhost = NULL;
	void *logctx = NULL;

	sEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    error = ssh_backend.init(NULL, &backhandle, &cfg,
		       cfg.host, cfg.port, &realhost, cfg.tcp_nodelay,
		       cfg.tcp_keepalives);
    ssh_backend.provide_logctx(backhandle, logctx);
	sfree(realhost);
    while ( !ssh_backend.sendok(backhandle) )
	{
		if ( eventLoop(INVALID_HANDLE_VALUE) < 0 )
			break;
    }

	CloseHandle(sEvent);

	sk_cleanup();
	return ( 0 );
}

void expire_timer_context(void *ctx)
{
}

long schedule_timer(int ticks, timer_fn_t fn, void *ctx)
{
	return 0;
}

void set_busy_status(void *frontend, int status)
{
}

void old_keyfile_warning(void)
{
}

void update_specials_menu(void *frontend)
{
}

void ldisc_update(void *frontend, int echo, int edit)
{
}

int term_ldisc(Terminal *term, int option)
{
    return FALSE;
}

void frontend_keypress(void *handle)
{
    return;
}

void notify_remote_exit(void *fe)
{
}

void connection_fatal(void *frontend, char *fmt, ...)
{
}

void fatalbox(char *fmt, ...)
{
}

void modalfatalbox(char *fmt, ...)
{
}

void logevent(void *frontend, const char *string)
{
	if ( string )
		printf("LOG: %s\n", string);
}

void log_packet(void *handle, int direction, int type,
		char *texttype, void *data, int len,
		int n_blanks, const struct logblank_t *blanks)
{
}

} /* extern "C" */