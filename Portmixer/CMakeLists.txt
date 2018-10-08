set(portMixerSources
        px_mixer.c
        )

if (APPLE)
    list(APPEND portMixerSources
            px_mac_coreaudio.c
            )
endif(APPLE)

if (WIN32)
    list(APPEND portMixerSources
            px_win_common.c
            px_win_endpoint.c
            px_win_wmme.c
            )
endif(WIN32)