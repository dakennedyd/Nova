/**
 * OpenAL cross platform audio library
 * Copyright (C) 1999-2007 by authors.
 * This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * Or go to http://www.gnu.org/copyleft/lgpl.html
 */

#include "config.h"

#include <cstdlib>
#include <cstring>
#include <cctype>

#include "AL/al.h"
#include "AL/alc.h"

#include "alMain.h"
#include "alcontext.h"
#include "alError.h"

AL_API ALboolean AL_APIENTRY alIsExtensionPresent(const ALchar *extName)
{
    ContextRef context{GetContextRef()};
    if(UNLIKELY(!context)) return AL_FALSE;

    if(!extName)
        SETERR_RETURN(context.get(), AL_INVALID_VALUE, AL_FALSE, "NULL pointer");

    size_t len{strlen(extName)};
    const char *ptr{context->ExtensionList};
    while(ptr && *ptr)
    {
        if(strncasecmp(ptr, extName, len) == 0 &&
           (ptr[len] == '\0' || isspace(ptr[len])))
            return AL_TRUE;

        if((ptr=strchr(ptr, ' ')) != nullptr)
        {
            do {
                ++ptr;
            } while(isspace(*ptr));
        }
    }

    return AL_FALSE;
}


AL_API ALvoid* AL_APIENTRY alGetProcAddress(const ALchar *funcName)
{
    if(!funcName) return nullptr;
    return alcGetProcAddress(nullptr, funcName);
}

AL_API ALenum AL_APIENTRY alGetEnumValue(const ALchar *enumName)
{
    if(!enumName) return static_cast<ALenum>(0);
    return alcGetEnumValue(nullptr, enumName);
}
