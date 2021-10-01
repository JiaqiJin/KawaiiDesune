#pragma once

#define KAWAII_NOOP() \
    {              \
        (void)0;   \
    }


// TODO move to platform dependent header
#define KAWAII_SET_CURRENT_THREAD_NAME(Name)                                \
    {                                                                    \
        HRESULT hr = ::SetThreadDescription(::GetCurrentThread(), Name); \
        assert(SUCCEEDED(hr));                                       \
    }