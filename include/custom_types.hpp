#pragma once

enum class CustomLuauTypeTags
{
    SCRIPTSIGNAL_TYPE = 1,
    SCRIPTCONNECTION_TYPE = 2,
    AUDIOTRACK_TYPE = 3,
    RNG_TYPE = 4,
};

#define SCRIPTSIGNAL_TYPENAME "scriptsignal"
#define SCRIPTCONNECTION_TYPENAME "scriptconnection"
#define AUDIOTRACK_TYPENAME "audiotrack"
#define RNG_TYPENAME "rng"