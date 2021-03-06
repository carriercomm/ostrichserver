/* 
   Copyright (c) 2014-2015 flashbuckets 

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 dated June, 1991, or
   (at your option) version 3 dated 29 June, 2007.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
     
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <common.h>
#include <config_parser.h>

// don't forget free the memory
static int
config_loadfile(char *filepath, size_t len, char **config_buf, 
    size_t *bufsize)
{
    struct  stat statbuf;
    int     ret;
    int     fd;
    size_t  filesize;
    ssize_t readsize;
    char   *buf;

    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        return CONF_ERR;
    }
    
    ret = fstat(fd, &statbuf);
    if (ret < 0) {
        return ret;
    }

    filesize = statbuf.st_size;

    buf = os_calloc(filesize + 1);
    if (!buf) {
        return CONF_ERR;
    }
    
    readsize = read(fd, buf, filesize);
    if (readsize < 0 || readsize != filesize) {
        return CONF_ERR;
    }

    buf[readsize] = 0;

    *config_buf = buf;
    
    *bufsize = readsize;
    
    return CONF_OK;
}

static int 
config_parse_buffer(char *buf, size_t size)
{
    return CONF_ERR;   
}

int config_parser(char *filepath, size_t len)
{
    char   *fbuf = NULL;
    size_t  bufsize = 0; 
    int     ret;
    
    ret = config_loadfile(filepath, len, &fbuf, &bufsize);
    if (ret != CONF_OK) {
        return ret;
    }

    ret = config_parse_buffer(fbuf, bufsize);

    os_free(fbuf);
    
    return CONF_OK;
}
