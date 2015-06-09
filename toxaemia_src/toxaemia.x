struct ToxConnectArgs {
	int BootstrapPort;
	string BootstrapAddress<>;
	string BootstrapKey<>;
	string Name<>;
	string Status<>;
};

program TOXAEMIA_PROG {
        version TOXAEMIA_VERS1 {
                int ToxConnect(ToxConnectArgs) = 1;
        } = 1;
} = 0x22159817;
