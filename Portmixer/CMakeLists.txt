set(portMixerSources
        px_mixer.c
        )

if (APPLE)
    list(APPEND portMixerSources
            px_mac_coreaudio.c
            )
endif(APPLE)