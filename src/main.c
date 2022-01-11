#define NOMINMAX            1
#define WIN32_LEAN_AND_MEAN 1
#define WIN32_MEAN_AND_LEAN 1
#define VC_EXTRALEAN        1
#include <windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN
#undef WIN32_MEAN_AND_LEAN
#undef VC_EXTRALEAN
#undef far
#undef near

typedef signed __int8  i8;
typedef signed __int16 i16;
typedef signed __int32 i32;
typedef signed __int64 i64;

typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;

typedef u64 umm;
typedef i64 imm;

// NOTE: required to remove CRT
void* memset(void* ptr, int value, umm size);
void* memcpy(void* rdst, const void* rsrc, u64 count);

#pragma function(memset)
#pragma function(memcpy)

void*
memset(void* ptr, int value, umm size)
{
    u8* bptr = ptr;
    u8 val   = (u8)value;
    
    for (umm i = 0; i < size; ++i)
    {
        *bptr++ = val;
    }
    
    return ptr;
}

void*
memcpy(void* rdst, const void* rsrc, u64 count)
{
    u8* dst = (u8*)rdst;
    const u8* src = (const u8*)rsrc;
    while (count--)
    {
        *dst++ = *src++;
    }
    return dst;
}


int _fltused;

int __stdcall
_DllMainCRTStartup(void* instance, u32 reason, void* reserved)
{
    return 1;
}

typedef u64 umm;
typedef i64 imm;

typedef float  f32;
typedef double f64;

typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#define U8_MAX  0xFF
#define U16_MAX 0xFFFF
#define U32_MAX 0xFFFFFFFF
#define U64_MAX 0xFFFFFFFFFFFFFFFF

#define I8_MIN  0xFF
#define I16_MIN 0xFFFF
#define I32_MIN 0xFFFFFFFF
#define I64_MIN 0xFFFFFFFFFFFFFFFF

#define I8_MAX  0x7F
#define I16_MAX 0x7FFF
#define I32_MAX 0x7FFFFFFF
#define I64_MAX 0x7FFFFFFFFFFFFFFF

typedef u8 bool;

#define false 0
#define true 1

typedef struct Buffer
{
    u8* data;
    u64 size;
} Buffer;

typedef Buffer String;

#define STRING(str) (String){ .data = (u8*)(str), .size = sizeof(str) - 1 }

#define Enum8(name)  u8
#define Enum16(name) u16
#define Enum32(name) u32
#define Enum64(name) u64

#define Flag8(name)  b8
#define Flag16(name) b16
#define Flag32(name) b32
#define Flag64(name) b64

#define _CONCAT(a, b) a##b
#define CONCAT(a, b) _CONCAT(a, b)
#define _STRINGIFY(a) #a
#define STRINGIFY(a) _STRINGIFY(a)

#define ASSERT(EX) ((EX) ? 1 : *(volatile int*)0)
#define NOT_IMPLEMENTED ASSERT(!"NOT_IMPLEMENTED")
#define INVALID_DEFAULT_CASE default: ASSERT(!"INVALID_DEFAULT_CASE"); break
#define INVALID_CODE_PATH ASSERT(!"INVALID_CODE_PATH")

#define OFFSETOF(element, type) (umm)&((type*)0)->element
#define ALIGNOF(T) OFFSETOF(t, struct { u8 b; T t; })

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(n) ((n) < 0 ? -(n) : (n))
#define SGN(n) ((n) < 0 ? -1 : ((n) == 0 ? 0 : 1))

#define KB(N) ((umm)(N) << 10)
#define MB(N) ((umm)(N) << 20)
#define GB(N) ((umm)(N) << 30)
#define TB(N) ((umm)(N) << 40)

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

#define internal static
#define global static

// NOTE: This is just a hack to work around a parsing bug in 4coder
#define TYPEDEF_FUNC(return_val, name, ...) typedef return_val (*name)(__VA_ARGS__)

internal inline bool
IsWhitespace(u8 c)
{
    return (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == '\n');
}

internal bool
String_Compare(String s0, String s1)
{
    if (s0.size == s1.size)
    {
        while (s0.size != 0 && *s0.data == *s1.data)
        {
            ++s0.data;
            --s0.size;
            
            ++s1.data;
            --s1.size;
        }
    }
    
    return (s0.size == 0 && s1.size == 0);
}

internal umm
String_FormatArgList(Buffer out, const char* format, va_list args)
{
    umm required_bytes = 0;
    
    for (char* scan = (char*)format; *scan; )
    {
        if (*scan == '%')
        {
            ++scan;
            
            switch (*scan)
            {
                case '%':
                {
                    if (out.size > required_bytes) out.data[required_bytes] = *scan;
                    ++required_bytes;
                    ++scan;
                } break;
                
                case 'u':
                case 'U':
                case 'i':
                case 'I':
                {
                    u64 val;
                    if      (*scan == 'u') val = va_arg(args, u32);
                    else if (*scan == 'U') val = va_arg(args, u64);
                    else
                    {
                        i64 i_val;
                        if      (*scan == 'i') i_val = va_arg(args, i32);
                        else                   i_val = va_arg(args, i64);
                        
                        if (i_val < 0)
                        {
                            if (required_bytes < out.size) out.data[required_bytes] = '-';
                            required_bytes += 1;
                            
                            i_val  = -i_val;
                        }
                        
                        val = (u64)i_val;
                    }
                    
                    ++scan;
                    
                    umm largest_place = 1;
                    u64 val_copy = val;
                    while (val_copy /= 10) largest_place *= 10;
                    
                    while (largest_place != 0)
                    {
                        if (required_bytes < out.size) out.data[required_bytes] = (val / largest_place) % 10 + '0';
                        largest_place  /= 10;
                        required_bytes += 1;
                    }
                } break;
                
                case 'x':
                case 'X':
                {
                    u64 val;
                    if      (*scan == 'x') val = va_arg(args, u32);
                    else                   val = va_arg(args, u64);
                    
                    umm largest_place = 1;
                    u64 val_copy = val;
                    while (val_copy /= 16) largest_place *= 16;
                    
                    while (largest_place != 0)
                    {
                        u8 digit = (val / largest_place) % 16;
                        
                        if (required_bytes < out.size) out.data[required_bytes] = (digit <= '9' ? digit + '0' : digit + 'A');
                        largest_place  /= 16;
                        required_bytes += 1;
                    }
                } break;
                
                case 'b':
                {
                    ++scan;
                    
                    bool val = va_arg(args, bool);
                    
                    char* str;
                    if (val) str = "true";
                    else     str = "false";
                    
                    for (; *str; ++str)
                    {
                        if (required_bytes < out.size) out.data[required_bytes] = *str;
                        required_bytes += 1;
                    }
                } break;
                
                case 'c':
                {
                    ++scan;
                    
                    char c = va_arg(args, char);
                    
                    if (required_bytes < out.size) out.data[required_bytes] = c;
                    required_bytes += 1;
                    
                } break;
                
                case 's':
                {
                    ++scan;
                    
                    char* str = va_arg(args, char*);
                    
                    for (; *str; ++str)
                    {
                        if (required_bytes < out.size) out.data[required_bytes] = *str;
                        required_bytes += 1;
                    }
                } break;
                
                case 'S':
                {
                    ++scan;
                    
                    String string = va_arg(args, String);
                    
                    for (umm i = 0; i < string.size; ++i)
                    {
                        if (required_bytes < out.size) out.data[required_bytes] = string.data[i];
                        required_bytes += 1;
                    }
                } break;
                
                case '*':
                {
                    ++scan;
                    
                    u8 c      = va_arg(args, u8);
                    u32 count = va_arg(args, u32);
                    
                    for (umm i = 0; i < count; ++i)
                    {
                        if (required_bytes < out.size) out.data[required_bytes] = c;
                        required_bytes += 1;
                    }
                } break;
                
                INVALID_DEFAULT_CASE;
            }
        }
        
        else
        {
            if (out.size > required_bytes) out.data[required_bytes] = *scan;
            ++required_bytes;
            ++scan;
        }
    }
    
    if (out.size > required_bytes) out.data[required_bytes] = 0;
    ++required_bytes;
    
    return required_bytes;
}

internal umm
String_Format(Buffer out, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    umm result = String_FormatArgList(out, format, args);
    va_end(args);
    
    return result;
}

internal bool
CString_HasStringPrefix(u8* cstring, String prefix)
{
    while (prefix.size != 0 && *cstring != 0 && prefix.data[0] == *cstring)
    {
        ++prefix.data;
        --prefix.size;
        ++cstring;
    }
    
    return (prefix.size == 0);
}

internal inline umm
RoundUpToAlignment(umm size, umm alignment)
{
    umm overshoot    = size + (alignment - 1);
    umm rounded_down = overshoot & ~(alignment - 1);
    
    return rounded_down;
}

internal inline umm
RoundDownToAlignment(umm size, umm alignment)
{
    umm rounded_down = size & ~(alignment - 1);
    
    return rounded_down;
}

internal inline void*
Align(void* ptr, u8 alignment)
{
    return (void*)RoundUpToAlignment((umm)ptr, alignment);
}

internal inline u8
AlignOffset(void* ptr, u8 alignment)
{
    return (u8)(RoundUpToAlignment((umm)ptr, alignment) - (umm)ptr);
}

internal inline void
Copy(void* src, void* dst, umm size)
{
    u8* bsrc = src;
    u8* bdst = dst;
    
    for (umm i = 0; i < size; ++i)
    {
        *bdst++ = *bsrc++;
    }
}

#define CopyStruct(src, dst) Copy((src), (dst), sizeof(*(src)))

typedef struct Memory_Arena_Marker
{
    u64 offset;
} Memory_Arena_Marker;

#define MEMORY_ARENA_PAGE_SIZE KB(4)

typedef struct Memory_Arena
{
    u64 base_address;
    u64 offset;
    u64 size;
} Memory_Arena;

internal void*
Arena_PushSize(Memory_Arena* arena, umm size, u8 alignment)
{
    u64 offset = arena->offset + AlignOffset((void*)(arena->base_address + arena->offset), alignment);
    
    if (offset + size > arena->size)
    {
        umm mem_to_commit = RoundUpToAlignment(offset + size - arena->size, KB(4)) + 2*KB(4);
        
        arena->size += mem_to_commit;
        
        if (!VirtualAlloc((void*)arena->base_address, arena->size, MEM_COMMIT, PAGE_READWRITE))
        {
            //// ERROR: Out of memory
            ExitProcess(0);
        }
    }
    
    arena->offset = offset + size;
    
    return (void*)(arena->base_address + offset);
}

internal inline void
Arena_PopSize(Memory_Arena* arena, umm size)
{
    ASSERT(arena->offset > size);
    arena->offset -= size;
}

internal inline void
Arena_Clear(Memory_Arena* arena)
{
    arena->offset = 0;
}

internal Memory_Arena_Marker
Arena_BeginTempMemory(Memory_Arena* arena)
{
    return (Memory_Arena_Marker){arena->offset};
}

internal void
Arena_EndTempMemory(Memory_Arena* arena, Memory_Arena_Marker marker)
{
    arena->offset = marker.offset;
}

internal umm
Arena_UsedSize(Memory_Arena* arena)
{
    return arena->offset;
}

Memory_Arena MainArena = {0};

internal void
Print(const char* format, ...)
{
    va_list args;
    
    Memory_Arena_Marker marker = Arena_BeginTempMemory(&MainArena);
    
    Buffer buffer;
    buffer.size = KB(4);
    buffer.data = Arena_PushSize(&MainArena, buffer.size, ALIGNOF(u8));
    
    va_start(args, format);
    umm required_size = String_FormatArgList(buffer, format, args);
    va_end(args);
    
    if (required_size > buffer.size)
    {
        Arena_EndTempMemory(&MainArena, marker);
        marker = Arena_BeginTempMemory(&MainArena);
        
        buffer.size = required_size;
        buffer.data = Arena_PushSize(&MainArena, required_size, ALIGNOF(u8));
        
        va_start(args, format);
        required_size = String_FormatArgList(buffer, format, args);
        va_end(args);
        
    }
    
    DWORD ignored;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer.data, (DWORD)required_size - 1, &ignored, 0);
    
    Arena_EndTempMemory(&MainArena, marker);
}

enum CODE_KIND
{
    Code_Invalid = 0,
    
    Code_Text,
    Code_C,
    Code_M,
    Code_Bash,
    Code_Batch,
    Code_Python,
    
    CODE_KIND_COUNT
};

typedef struct File_Info
{
    Enum8(CODE_KIND) kind;
    String name;
    u32 file_size;
    u32 total_lines;
    u32 commented_lines;
    u32 empty_lines;
    u32 computed_loc;
} File_Info;

internal void
SortFileInfos(File_Info* infos, umm count)
{
    if (count != 0)
    {
        umm pivot = infos[count - 1].computed_loc;
        
        File_Info* cursor = infos;
        for (File_Info* scan = infos; scan < infos + count; ++scan)
        {
            if (scan->computed_loc >= pivot)
            {
                File_Info temp = *scan;
                *scan   = *cursor;
                *cursor = temp;
                
                ++cursor;
            }
        }
        
        SortFileInfos(infos, (cursor - 1) - infos);
        SortFileInfos(cursor, (infos + count) - cursor);
    }
}

void __stdcall
WinMainCRTStartup()
{
    MainArena = (Memory_Arena){
        .base_address = (u64)VirtualAlloc(0, GB(1), MEM_RESERVE, PAGE_NOACCESS),
        .offset       = 0,
        .size         = 0,
    };
    
    Memory_Arena file_info_arena = (Memory_Arena){
        .base_address = (u64)VirtualAlloc(0, GB(1), MEM_RESERVE, PAGE_NOACCESS),
        .offset       = 0,
        .size         = 0,
    };
    
    u8* command_line = (u8*)GetCommandLineA();
    
    u8* scan = command_line;
    while (*scan != 0 && !IsWhitespace(*scan)) ++scan;
    while (*scan != 0 &&  IsWhitespace(*scan)) ++scan;
    
    u8* path = scan;
    
    while (*scan != 0 && !IsWhitespace(*scan)) ++scan;
    
    if (*path == 0) Print("Invalid Arguments: Missing search path, usage dcloc search\\path\\with\\otional\\wildcards");
    else if (*scan != 0) Print("Invalid Arguments: Extraneous arguments, usage dcloc search\\path\\with\\otional\\wildcards");
    else
    {
        WIN32_FIND_DATAA find_data = {0};
        
        HANDLE find_handle = FindFirstFileA((LPCSTR)path, &find_data);
        
        if (find_handle == 0 || !PathRemoveFileSpecA((LPSTR)path) ||
            !SetCurrentDirectory((LPCSTR)path))
        {
            Print("ERROR: Failed to query files");
        }
        
        else
        {
            umm file_info_count = 0;
            
            do
            {
                u8* filename        = (u8*)find_data.cFileName;
                umm filename_length = 0;
                
                String extension = { .data = filename };
                for (u8* filename_scan = filename; *filename_scan != 0; ++filename_scan)
                {
                    if (*filename_scan == '.')
                    {
                        extension.data = filename_scan + 1;
                        extension.size = 0;
                    }
                    
                    else extension.size += 1;
                    
                    filename_length += 1;
                }
                
                String extension_kinds[CODE_KIND_COUNT][4] = {
                    [Code_Text]   = { STRING("txt") },
                    [Code_C]      = { STRING("c"), STRING("h"), STRING("cc") },
                    [Code_M]      = { STRING("m")   },
                    [Code_Bash]   = { STRING("sh")  },
                    [Code_Batch]  = { STRING("bat") },
                    [Code_Python] = { STRING("py")  },
                };
                
                Enum8(CODE_KIND) code_kind = Code_Invalid;
                
                for (umm i = 0; i < ARRAY_SIZE(extension_kinds) && code_kind == Code_Invalid; ++i)
                {
                    for (umm j = 0; j < ARRAY_SIZE(extension_kinds[i]) && extension_kinds[i][j].data != 0; ++j)
                    {
                        if (String_Compare(extension, extension_kinds[i][j]))
                        {
                            code_kind = (u8)i;
                        }
                    }
                }
                
                if (code_kind != Code_Invalid)
                {
                    umm size = ((u64)find_data.nFileSizeHigh << 32) | find_data.nFileSizeLow;
                    
                    if (size > U32_MAX) Print("WARNING: skipped reading %s, file too big", filename);
                    else
                    {
                        File_Info* info = Arena_PushSize(&file_info_arena, sizeof(File_Info), ALIGNOF(File_Info));
                        file_info_count += 1;
                        
                        info->kind = code_kind;
                        info->file_size = (u32)size;
                        
                        info->name.size = filename_length;
                        info->name.data = Arena_PushSize(&MainArena, info->name.size, ALIGNOF(u8));
                        Copy(filename, info->name.data, info->name.size);
                        
                        struct { bool exists; String start; } single_line_comment_styles[CODE_KIND_COUNT] = {
                            [Code_Text]   = { .exists = false },
                            [Code_C]      = { .exists = true, .start = STRING("//")  },
                            [Code_M]      = { .exists = true, .start = STRING("//")  },
                            [Code_Bash]   = { .exists = true, .start = STRING("#")   },
                            [Code_Batch]  = { .exists = true, .start = STRING("REM") },
                            [Code_Python] = { .exists = true, .start = STRING("#")   },
                        };
                        
                        struct { bool exists; String start; String end; } multi_line_comment_styles[CODE_KIND_COUNT] = {
                            [Code_Text]   = { .exists = false },
                            [Code_C]      = { .exists = true, .start = STRING("/*"), .end = STRING("*/") },
                            [Code_M]      = { .exists = true, .start = STRING("/*"), .end = STRING("*/") },
                            [Code_Bash]   = { .exists = false },
                            [Code_Batch]  = { .exists = false },
                            [Code_Python] = { .exists = false },
                        };
                        
                        Memory_Arena_Marker marker = Arena_BeginTempMemory(&MainArena);
                        
                        u8* file_content = Arena_PushSize(&MainArena, info->file_size + 1, ALIGNOF(u8));
                        
                        HANDLE file_handle = CreateFile((LPCSTR)filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
                        
                        DWORD bytes_read;
                        if (file_handle == INVALID_HANDLE_VALUE || !ReadFile(file_handle, file_content, info->file_size, &bytes_read, 0) ||
                            bytes_read != info->file_size)
                        {
                            Print("ERROR: Failed to read %s\n", filename);
                        }
                        
                        else
                        {
                            file_content[info->file_size] = 0;
                            
                            for (scan = file_content; *scan != 0; ++scan)
                            {
                                while (*scan != 0 && *scan != '\n' && IsWhitespace(*scan)) ++scan;
                                
                                if (single_line_comment_styles[info->kind].exists && CString_HasStringPrefix(scan, single_line_comment_styles[info->kind].start))
                                {
                                    while (*scan != 0 && *scan != '\n') ++scan;
                                    if (*scan == '\n')
                                    {
                                        info->commented_lines += 1;
                                        info->total_lines     += 1;
                                        ++scan;
                                    }
                                }
                                
                                else if (multi_line_comment_styles[info->kind].exists &&
                                         CString_HasStringPrefix(scan, multi_line_comment_styles[info->kind].start))
                                {
                                    String start = multi_line_comment_styles[info->kind].start;
                                    String end   = multi_line_comment_styles[info->kind].end;
                                    
                                    umm nest_level = 0;
                                    while (*scan != 0)
                                    {
                                        if (CString_HasStringPrefix(scan, start))
                                        {
                                            nest_level += 1;
                                            
                                            scan += start.size;
                                        }
                                        
                                        else if (CString_HasStringPrefix(scan, end))
                                        {
                                            nest_level -= 1;
                                            
                                            scan += end.size;
                                        }
                                        
                                        else
                                        {
                                            if (*scan == '\n') info->commented_lines += 1;
                                            
                                            scan += 1;
                                        }
                                        
                                        if (nest_level == 0) break;
                                    }
                                    
                                    while (*scan != 0 && *scan != '\n') ++scan;
                                    if (*scan == '\n')
                                    {
                                        info->commented_lines += 1;
                                        info->total_lines     += 1;
                                        ++scan;
                                    }
                                }
                                
                                else
                                {
                                    if (*scan == '\n')
                                    {
                                        info->empty_lines += 1;
                                        info->total_lines += 1;
                                        ++scan;
                                    }
                                    
                                    else
                                    {
                                        while (*scan != 0 && *scan != '\n') ++scan;
                                        if (*scan == '\n')
                                        {
                                            info->total_lines += 1;
                                            ++scan;
                                        }
                                    }
                                }
                            }
                            
                            info->computed_loc = info->total_lines - (info->commented_lines + info->empty_lines);
                        }
                        
                        CloseHandle(file_handle);
                        
                        Arena_EndTempMemory(&MainArena, marker);
                    }
                }
            } while (FindNextFile(find_handle, &find_data));
            
            FindClose(find_handle);
            
            if (file_info_count == 0) Print("No files found");
            else
            {
                File_Info* file_infos = (File_Info*)file_info_arena.base_address;
                SortFileInfos(file_infos, file_info_count);
                
                u32 longest_name            = sizeof("File Name") - 1;
                u32 longest_total_lines     = sizeof("Total Lines") - 1;
                u32 longest_commented_lines = sizeof("Commented Lines") - 1;
                u32 longest_empty_lines     = sizeof("Empty lines") - 1;
                u32 longest_computed_loc    = sizeof("Computed LoC") - 1;
                for (umm i = 0; i < file_info_count; ++i)
                {
                    longest_name            = (u32)MAX(longest_name, file_infos[i].name.size);
                    longest_total_lines     = (u32)MAX(longest_total_lines, String_Format((Buffer){0}, "%u", file_infos[i].total_lines) - 1);
                    longest_commented_lines = (u32)MAX(longest_commented_lines, String_Format((Buffer){0}, "%u", file_infos[i].commented_lines) - 1);
                    longest_empty_lines     = (u32)MAX(longest_empty_lines, String_Format((Buffer){0}, "%u", file_infos[i].empty_lines) - 1);
                    longest_computed_loc    = (u32)MAX(longest_computed_loc, String_Format((Buffer){0}, "%u", file_infos[i].computed_loc) - 1);
                }
                
                {
                    u32 pad[10] = {
                        (longest_name            - (sizeof("File Name")       - 1))     / 2,
                        (longest_name            - (sizeof("File Name")       - 1) + 1) / 2,
                        (longest_total_lines     - (sizeof("Total Lines")     - 1))     / 2,
                        (longest_total_lines     - (sizeof("Total Lines")     - 1) + 1) / 2,
                        (longest_commented_lines - (sizeof("Commented Lines") - 1))     / 2,
                        (longest_commented_lines - (sizeof("Commented Lines") - 1) + 1) / 2,
                        (longest_empty_lines     - (sizeof("Empty Lines")     - 1))     / 2,
                        (longest_empty_lines     - (sizeof("Empty Lines")     - 1) + 1) / 2,
                        (longest_computed_loc    - (sizeof("Computed LoC")    - 1))     / 2,
                        (longest_computed_loc    - (sizeof("Computed LoC")    - 1) + 1) / 2,
                    };
                    
                    Print("+%*+%*+%*+%*+%*+\n", '-', longest_name + 2, '-', longest_total_lines + 2, '-', longest_commented_lines + 2, '-', longest_empty_lines + 2, '-', longest_computed_loc + 2);
                    Print("| %*%s%* | %*%s%* | %*%s%* | %*%s%* | %*%s%* |\n", ' ', pad[0], "File Name", ' ', pad[1], ' ', pad[2], "Total Lines", ' ', pad[3], ' ', pad[4], "Commented Lines", ' ', pad[5], ' ', pad[6], "Empty Lines", ' ', pad[7], ' ', pad[8], "Computed LoC", ' ', pad[9]);
                    Print("+%*+%*+%*+%*+%*+\n", '-', longest_name + 2, '-', longest_total_lines + 2, '-', longest_commented_lines + 2, '-', longest_empty_lines + 2, '-', longest_computed_loc + 2);
                }
                
                umm total_computed_loc[CODE_KIND_COUNT] = {0};
                umm total_lines[CODE_KIND_COUNT]        = {0};
                
                for (umm i = 0; i < file_info_count; ++i)
                {
                    File_Info* info = &file_infos[i];
                    
                    u32 total_lines_length     = (u32)String_Format((Buffer){0}, "%u", info->total_lines) - 1;
                    u32 commented_lines_length = (u32)String_Format((Buffer){0}, "%u", info->commented_lines) - 1;
                    u32 empty_lines_length     = (u32)String_Format((Buffer){0}, "%u", info->empty_lines) - 1;
                    u32 computed_loc_length    = (u32)String_Format((Buffer){0}, "%u", info->computed_loc) - 1;
                    
                    u32 pad[10] = {
                        (longest_name            - (u32)info->name.size)       / 2,
                        (longest_name            - (u32)info->name.size   + 1) / 2,
                        (longest_total_lines     - total_lines_length)         / 2,
                        (longest_total_lines     - total_lines_length     + 1) / 2,
                        (longest_commented_lines - commented_lines_length)     / 2,
                        (longest_commented_lines - commented_lines_length + 1) / 2,
                        (longest_empty_lines     - empty_lines_length)         / 2,
                        (longest_empty_lines     - empty_lines_length     + 1) / 2,
                        (longest_computed_loc    - computed_loc_length)        / 2,
                        (longest_computed_loc    - computed_loc_length    + 1) / 2,
                    };
                    
                    Print("| %*%S%* | %*%u%* | %*%u%* | %*%u%* | %*%u%* |\n", ' ', pad[0], info->name, ' ', pad[1], ' ', pad[2], info->total_lines, ' ', pad[3], ' ', pad[4], info->commented_lines, ' ', pad[5], ' ', pad[6], info->empty_lines, ' ', pad[7], ' ', pad[8], info->computed_loc, ' ', pad[9]);
                    
                    Print("+%*+%*+%*+%*+%*+\n", '-', longest_name + 2, '-', longest_total_lines + 2, '-', longest_commented_lines + 2, '-', longest_empty_lines + 2, '-', longest_computed_loc + 2);
                    
                    total_computed_loc[info->kind] += info->computed_loc;
                    total_lines[info->kind]        += info->total_lines;
                }
                
                String lang_name[CODE_KIND_COUNT] = {
                    [Code_Text]   = STRING("Text"),
                    [Code_C]      = STRING("C"),
                    [Code_M]      = STRING("M"),
                    [Code_Bash]   = STRING("Bash"),
                    [Code_Batch]  = STRING("Batch"),
                    [Code_Python] = STRING("Python"),
                };
                
                Print("\nTotal Computed LoC\n");
                for (umm i = 0; i < ARRAY_SIZE(total_computed_loc); ++i)
                {
                    if (total_computed_loc[i] != 0)
                    {
                        Print("%S: %U\n", lang_name[i], total_computed_loc[i]);
                    }
                }
                
                Print("\nTotal Lines\n");
                for (umm i = 0; i < ARRAY_SIZE(total_lines); ++i)
                {
                    if (total_lines[i] != 0)
                    {
                        Print("%S: %U\n", lang_name[i], total_lines[i]);
                    }
                }
            }
        }
    }
    
    ExitProcess(0);
}
