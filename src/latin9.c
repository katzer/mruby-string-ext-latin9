/* MIT License
 *
 * Copyright (c) 2018 Sebastian Katzer, appPlant GmbH
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// https://stackoverflow.com/questions/11258019/conversion-from-iso-8859-15-latin9-to-utf-8

#include "mruby.h"
#include "mruby/string.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

/*
 * Create a dynamically allocated copy of string,
 * changing the encoding from ISO-8859-15 to UTF-8.
 */
static mrb_value latin9_to_utf8(mrb_state *mrb, const char *const string)
{
    size_t  n = 0;
    mrb_value out;

    if (string) {
        const unsigned char  *s = (const unsigned char *)string;

        while (*s)
            if (*s < 128) {
                s++;
                n += 1;
            } else
            if (*s == 164) {
                s++;
                n += 3;
            } else {
                s++;
                n += 2;
            }
    }
    out = mrb_str_new_capa(mrb, n + 1);

    if (n) {
        const unsigned char  *s = (const unsigned char *)string;
        unsigned char        *d = (unsigned char *)RSTRING_PTR(out);

        while (*s)
            if (*s < 128) {
                *(d++) = *(s++);
            } else
            if (*s < 192) switch (*s) {
                case 164: *(d++) = 226; *(d++) = 130; *(d++) = 172; s++; break;
                case 166: *(d++) = 197; *(d++) = 160; s++; break;
                case 168: *(d++) = 197; *(d++) = 161; s++; break;
                case 180: *(d++) = 197; *(d++) = 189; s++; break;
                case 184: *(d++) = 197; *(d++) = 190; s++; break;
                case 188: *(d++) = 197; *(d++) = 146; s++; break;
                case 189: *(d++) = 197; *(d++) = 147; s++; break;
                case 190: *(d++) = 197; *(d++) = 184; s++; break;
                default:  *(d++) = 194; *(d++) = *(s++); break;
            } else {
                *(d++) = 195;
                *(d++) = *(s++) - 64;
            }

        *d = 0;
        mrb_str_resize(mrb, out, (char*)d - RSTRING_PTR(out));
    }

    /* Done. Remember to free() the resulting string when no longer needed. */
    return out;
}

/*
 * Create a dynamically allocated copy of string,
 * changing the encoding from UTF-8 to ISO-8859-15.
 * Unsupported code points are ignored.
 */
static mrb_value utf8_to_latin9(mrb_state *mrb, const char *const string)
{
    size_t    size = 0;
    size_t    used = 0;
    mrb_value out = mrb_str_new(mrb, NULL, 0);
    unsigned char *result = (unsigned char*)RSTRING_PTR(out);

    if (string) {
        const unsigned char  *s = (const unsigned char *)string;

        while (*s) {

            if (used >= size) {
                mrb_str_resize(mrb, out, size);
                result = (unsigned char*)RSTRING_PTR(out);
            }

            if (*s < 128) {
                result[used++] = *(s++);
                continue;

            } else
            if (s[0] == 226 && s[1] == 130 && s[2] == 172) {
                result[used++] = 164;
                s += 3;
                continue;

            } else
            if (s[0] == 194 && s[1] >= 128 && s[1] <= 191) {
                result[used++] = s[1];
                s += 2;
                continue;

            } else
            if (s[0] == 195 && s[1] >= 128 && s[1] <= 191) {
                result[used++] = s[1] + 64;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 160) {
                result[used++] = 166;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 161) {
                result[used++] = 168;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 189) {
                result[used++] = 180;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 190) {
                result[used++] = 184;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 146) {
                result[used++] = 188;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 147) {
                result[used++] = 189;
                s += 2;
                continue;

            } else
            if (s[0] == 197 && s[1] == 184) {
                result[used++] = 190;
                s += 2;
                continue;

            }

            if (s[0] >= 192 && s[0] < 224 &&
                s[1] >= 128 && s[1] < 192) {
                s += 2;
                continue;
            } else
            if (s[0] >= 224 && s[0] < 240 &&
                s[1] >= 128 && s[1] < 192 &&
                s[2] >= 128 && s[2] < 192) {
                s += 3;
                continue;
            } else
            if (s[0] >= 240 && s[0] < 248 &&
                s[1] >= 128 && s[1] < 192 &&
                s[2] >= 128 && s[2] < 192 &&
                s[3] >= 128 && s[3] < 192) {
                s += 4;
                continue;
            } else
            if (s[0] >= 248 && s[0] < 252 &&
                s[1] >= 128 && s[1] < 192 &&
                s[2] >= 128 && s[2] < 192 &&
                s[3] >= 128 && s[3] < 192 &&
                s[4] >= 128 && s[4] < 192) {
                s += 5;
                continue;
            } else
            if (s[0] >= 252 && s[0] < 254 &&
                s[1] >= 128 && s[1] < 192 &&
                s[2] >= 128 && s[2] < 192 &&
                s[3] >= 128 && s[3] < 192 &&
                s[4] >= 128 && s[4] < 192 &&
                s[5] >= 128 && s[5] < 192) {
                s += 6;
                continue;
            }

            s++;
        }

        mrb_str_resize(mrb, out, used);
    }

    return out;
}

static mrb_value
mrb_f_to_latin9 (mrb_state *mrb, mrb_value self)
{
    return utf8_to_latin9(mrb, RSTRING_PTR(self));
}

static mrb_value
mrb_f_from_latin9 (mrb_state *mrb, mrb_value self)
{
    return latin9_to_utf8(mrb, RSTRING_PTR(self));
}

void
mrb_mruby_string_ext_latin9_gem_init (mrb_state *mrb)
{
    struct RClass *str = mrb_class_get(mrb, "String");

    mrb_define_method(mrb, str, "to_latin9",   mrb_f_to_latin9,   MRB_ARGS_NONE());
    mrb_define_method(mrb, str, "from_latin9", mrb_f_from_latin9, MRB_ARGS_NONE());
}

void
mrb_mruby_string_ext_latin9_gem_final (mrb_state *mrb)
{

}
