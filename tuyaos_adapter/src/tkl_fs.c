/**
 * @file tkl_fs.c
 * @brief the default weak implements of tuya os file system, this implement only used when OS=linux
 * @version 0.1
 * @date 2019-08-15
 * 
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 * 
 */
#include "tkl_fs.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

/**
* @brief Make directory
*
* @param[in] path: path of directory
*
* @note This API is used for making a directory
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fs_mkdir(const char* path)
{
    return mkdir(path, 0755);
}

/**
* @brief Remove directory
*
* @param[in] path: path of directory
*
* @note This API is used for removing a directory
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fs_remove(const char* path)
{
    return remove(path);
}

/**
* @brief Get file mode
*
* @param[in] path: path of directory
* @param[out] mode: bit attibute of directory
*
* @note This API is used for getting file mode.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fs_mode(const char* path, uint32_t* mode)
{
    if (mode == NULL) {
        return -1;
    }

    struct stat statbuf = {0};
    int ret = lstat(path, &statbuf);
    if (ret != 0) {
        return ret;
    }

    *mode = statbuf.st_mode;
    return 0;
}

/**
* @brief Check whether the file or directory exists
*
* @param[in] path: path of directory
* @param[out] is_exist: the file or directory exists or not
*
* @note This API is used to check whether the file or directory exists.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fs_is_exist(const char* path, BOOL_T* is_exist)
{
    if (is_exist == NULL) {
        return -1;
    }

    int ret = access(path, F_OK);
    *is_exist = (ret == 0);
    return 0;
}

/**
* @brief File rename
*
* @param[in] path_old: old path of directory
* @param[in] path_new: new path of directory
*
* @note This API is used to rename the file.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fs_rename(const char* path_old, const char* path_new)
{
    return rename(path_old, path_new);
}


/**
* @brief Open directory
*
* @param[in] path: path of directory
* @param[out] dir: handle of directory
*
* @note This API is used to open a directory
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_dir_open(const char* path, TUYA_DIR* dir)
{
    if (dir == NULL) {
        return -1;
    }

    DIR* d = opendir(path);
    if (d == NULL) {
        return -1;
    }

    *dir = d;
    return 0;
}

/**
* @brief Close directory
*
* @param[in] dir: handle of directory
*
* @note This API is used to close a directory
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_dir_close(TUYA_DIR dir)
{
    DIR* dirp = (DIR*)dir;
    return closedir(dirp);
}

/**
* @brief Read directory
*
* @param[in] dir: handle of directory
* @param[out] info: file information
*
* @note This API is used to read a directory.
* Read the file information of the current node, and the internal pointer points to the next node.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_dir_read(TUYA_DIR dir, TUYA_FILEINFO* info)
{
    if (info == NULL) {
        return -1;
    }

    DIR* dirp = (DIR*)dir;
    struct dirent* dp = readdir(dirp);
    if (dp == NULL) {
        return -1;
    }

    *info = dp;
    return 0;
}

/**
* @brief Get the name of the file node
*
* @param[in] info: file information
* @param[out] name: file name
*
* @note This API is used to get the name of the file node.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_dir_name(TUYA_FILEINFO info, const char** name)
{
    if (name == NULL) {
        return -1;
    }

    struct dirent* dp = (struct dirent*)info;
    if (dp == NULL) {
        return -1;
    }

    *name = dp->d_name;
    return 0;
}

/**
* @brief Check whether the node is a directory
*
* @param[in] info: file information
* @param[out] is_dir: is directory or not
*
* @note This API is used to check whether the node is a directory.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_dir_is_directory(TUYA_FILEINFO info, BOOL_T* is_dir)
{
    if (is_dir == NULL) {
        return -1;
    }

    struct dirent* dp = (struct dirent*)info;
    *is_dir = (dp->d_type == DT_DIR);
    return 0;
}

/**
* @brief Check whether the node is a normal file
*
* @param[in] info: file information
* @param[out] is_regular: is normal file or not
*
* @note This API is used to check whether the node is a normal file.
*
* @return 0 on success. Others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_dir_is_regular(TUYA_FILEINFO info, BOOL_T* is_regular)
{
    if (is_regular == NULL) {
        return -1;
    }

    struct dirent* dp = (struct dirent*)info;
    *is_regular = (dp->d_type == DT_REG);
    return 0;
}

/**
* @brief Open file
*
* @param[in] path: path of file
* @param[in] mode: file open mode: "r","w"...
*
* @note This API is used to open a file
*
* @return the file handle, NULL means failed
*/
TUYA_WEAK_ATTRIBUTE TUYA_FILE tkl_fopen(const char* path, const char* mode)
{
    return fopen(path, mode);
}

/**
* @brief Close file
*
* @param[in] file: file handle
*
* @note This API is used to close a file
*
* @return 0 on success. EOF on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fclose(TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fclose(fp);
}

/**
* @brief Read file
*
* @param[in] buf: buffer for reading file
* @param[in] bytes: buffer size
* @param[in] file: file handle
*
* @note This API is used to read a file
*
* @return the bytes read from file
*/
TUYA_WEAK_ATTRIBUTE int tkl_fread(void* buf, int bytes, TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fread(buf, 1, bytes, fp);
}

/**
* @brief write file
*
* @param[in] buf: buffer for writing file
* @param[in] bytes: buffer size
* @param[in] file: file handle
*
* @note This API is used to write a file
*
* @return the bytes write to file
*/
TUYA_WEAK_ATTRIBUTE int tkl_fwrite(void* buf, int bytes, TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fwrite(buf, 1, bytes, fp);
}

/**
* @brief write buffer to flash
*
* @param[in] fd: file fd
*
* @note This API is used to write buffer to flash
*
* @return 0 on success. others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fsync(int fd)
{
    return fsync(fd);
}

/**
* @brief Read string from file
*
* @param[in] buf: buffer for reading file
* @param[in] len: buffer size
* @param[in] file: file handle
*
* @note This API is used to read string from file
*
* @return the content get from file, NULL means failed
*/
TUYA_WEAK_ATTRIBUTE char* tkl_fgets(char* buf, int len, TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fgets(buf, len, fp);
}

/**
* @brief Check wheather to reach the end fo the file
*
* @param[in] file: file handle
*
* @note This API is used to check wheather to reach the end fo the file
*
* @return 0 on not eof, others on eof
*/
TUYA_WEAK_ATTRIBUTE int tkl_feof(TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return feof(fp);
}

/**
* @brief Seek to the offset position of the file
*
* @param[in] file: file handle
* @param[in] offs: offset
* @param[in] whence: seek start point mode
*
* @note This API is used to seek to the offset position of the file.
*
* @return 0 on success, others on failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fseek(TUYA_FILE file, int64_t offs, int whence)
{
    FILE* fp = (FILE*)file;
    return fseek(fp, offs, whence);
}

/**
* @brief Get current position of file
*
* @param[in] file: file handle
*
* @note This API is used to get current position of file.
*
* @return the current offset of the file
*/
TUYA_WEAK_ATTRIBUTE int64_t tkl_ftell(TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return ftell(fp);
}

/**
* @brief Get file size
*
* @param[in] filepath file path + file name
*
* @note This API is used to get the size of file.
*
* @return the sizeof of file
*/
TUYA_WEAK_ATTRIBUTE int tkl_fgetsize(const char *filepath)
{
    struct stat statbuf;
    stat(filepath, &statbuf);
    int size = statbuf.st_size;

    return size;
}

/**
* @brief Judge if the file can be access
*
* @param[in] filepath file path + file name
*
* @param[in] mode access mode
*
* @note This API is used to access one file.
*
* @return 0 success,-1 failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_faccess(const char *filepath, int mode)
{
    return access(filepath, mode);
}

/**
* @brief read the next character from stream
*
* @param[in] file char stream
*
* @note This API is used to get one char from stream.
*
* @return as an unsigned char cast to a int ,or EOF on end of file or error
*/
TUYA_WEAK_ATTRIBUTE int tkl_fgetc(TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fgetc(fp);
}

/**
* @brief flush the IO read/write stream
*
* @param[in] file char stream
*
* @note This API is used to flush the IO read/write stream.
*
* @return 0 success,-1 failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_fflush(TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fflush(fp);
}

/**
* @brief get the file fd
*
* @param[in] file char stream
*
* @note This API is used to get the file fd.
*
* @return the file fd
*/
TUYA_WEAK_ATTRIBUTE int tkl_fileno(TUYA_FILE file)
{
    FILE* fp = (FILE*)file;
    return fileno(fp);
}

/**
* @brief truncate one file according to the length
*
* @param[in] fd file description
*
* @param[in] length the length want to truncate
*
* @note This API is used to truncate one file.
*
* @return 0 success,-1 failed
*/
TUYA_WEAK_ATTRIBUTE int tkl_ftruncate(int fd, uint64_t length)
{
    return ftruncate(fd, length);
}
