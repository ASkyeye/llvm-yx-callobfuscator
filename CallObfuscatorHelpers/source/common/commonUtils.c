/**
 * @file commonUtils.c
 * @author Alejandro González (@httpyxel)
 * @brief Common functionality that are used across the project.
 * @version 0.1
 * @date 2024-01-14
 *
 * @copyright
 *   Copyright (C) 2024  Alejandro González
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "common/commonUtils.h"

PVOID __callobf_memset(const PVOID p_bytes, const BYTE c, size_t len)
{
    for (size_t i = 0; i < len; i++)
        (((PCHAR)p_bytes)[i] = c);

    return p_bytes;
}

DWORD __callobf_rand(PDWORD next) // RAND_MAX assumed as 256 + 20
{
    *next = *next * 1103515245 + 12345;
    return ((DWORD)(*next / 65536) % 0x7f) + 0x20;
}

VOID __callobf_srand(PDWORD next, DWORD seed)
{
    *next = seed;
}

UINT32 __callobf_hashA(const PCHAR p_str)
{
    UINT h;
    PCHAR p;
    CHAR c;

    h = 0;
    for (p = p_str; *p != '\0'; p++)
    {
        c = (*p >= 65 && *p <= 90) ? *p + 32 : *p;
        h = HASH_MULTIPLIER * h + c;
    }
    return h;
}

UINT32 __callobf_hashW(const PWCHAR p_str)
{
    UINT h;
    PWCHAR p;
    WCHAR c;

    h = 0;
    for (p = p_str; *p != 0; p++)
    {
        c = (*p >= 65 && *p <= 90) ? *p + 32 : *p;
        h = HASH_MULTIPLIER * h + c;
    }
    return h;
}

UINT32 __callobf_hashU(const PUNICODE_STRING p_str)
{
    UINT32 h;
    PWCHAR p;
    WCHAR c;
    DWORD_PTR p_end = ((DWORD_PTR)p_str->Buffer) + p_str->Length;

    h = 0;
    for (p = p_str->Buffer; (DWORD_PTR)p < p_end; p++)
    {
        c = (*p >= 65 && *p <= 90) ? *p + 32 : *p;
        h = HASH_MULTIPLIER * h + c;
    }
    return h;
}

PVOID __callobf_findBytes(
    _In_ const PVOID startAddr,
    _In_ const PVOID endAddr,
    _In_ const PBYTE bytes,
    _In_ const PBYTE mask,
    _In_ const DWORD byteCount)
{
    BYTE checkByte = 0;
    BYTE maskByte = 0;
    DWORD matches = 0;

    for (DWORD_PTR j = (DWORD_PTR)startAddr; j < ((DWORD_PTR)endAddr - byteCount); j++)
    {
        checkByte = *(PBYTE)((DWORD_PTR)bytes + matches);
        maskByte = *(PBYTE)((DWORD_PTR)mask + matches);
        if (checkByte == *(PBYTE)j || maskByte == 0)
        {
            if (++matches == byteCount)
                return (PVOID)(j - (matches - 1));
        }
        else
        {
            matches = 0;
        }
    }
    return NULL;
}

PVOID __callobf_getStackBase()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    return ((PNT_TIB)NtCurrentTeb())->StackBase;
#pragma GCC diagnostic pop
}

PVOID __callobf_getStackLimit()
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    return ((PNT_TIB)NtCurrentTeb())->StackLimit;
#pragma GCC diagnostic pop
}

DWORD32 __callobf_lfsrXorShift32(DWORD32 prevVal)
{
    prevVal ^= prevVal >> 13;
    prevVal ^= prevVal << 17;
    prevVal ^= prevVal >> 5;
    return prevVal;
}