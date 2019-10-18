#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

char *extract_filename(char *);
char *get_mode(struct stat);
char *get_owner_name(struct stat);
char *get_group_name(struct stat);
char *format_time(__time_t *time);
void mystat(char *path);

int main(int argc, char *argv[])
{
    int i;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <pathname> ... <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (i = 1; i < argc; i++)
    {
        mystat(argv[i]);
    }

    return 0;
}

void mystat(char *path)
{
    struct stat sb;
    char *symthere;
    char *filename;
    char *sym_extracted;
    struct stat other_stat;
    char *mode_text;
    char *owner;
    char *group;
    char *atime;
    char *mtime;
    char *ctime;

    if (lstat(path, &sb) == -1)
    {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    symthere = malloc(1024);
    memset(symthere, 0, 1024);
    if ((sb.st_mode & S_IFMT) == S_IFLNK)
    {
        readlink(path, symthere, 1023);
    }

    filename = extract_filename(path);

    printf("File: %s\n", filename);
    printf("  Filetype:            ");
    switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("     block device\n");
        break;
    case S_IFCHR:
        printf("     character device\n");
        break;
    case S_IFDIR:
        printf("     directory\n");
        break;
    case S_IFIFO:
        printf("     FIFO/pipe\n");
        break;
    case S_IFLNK:
        printf("     Symbolic link");
        if (stat(symthere, &other_stat) != 0)
        {
            printf(" - with dangling destination\n");
        }
        else
        {
            sym_extracted = extract_filename(symthere);
            printf(" -> %s\n", sym_extracted);
        }

        break;
    case S_IFREG:
        printf("     regular file\n");
        break;
    case S_IFSOCK:
        printf("     socket\n");
        break;
    default:
        printf("     unknown?\n");
        break;
    }
    free(symthere);

    printf("  Device ID Number:         %lu\n", sb.st_dev);
    printf("  I-node number:            %lu\n", sb.st_ino);
    mode_text = get_mode(sb);
    printf("  Mode:                     %s                  (%o in octal)\n", mode_text, sb.st_mode & 0b111111111);
    free(mode_text);
    printf("  Link count:               %ld\n", (long)sb.st_nlink);
    owner = get_owner_name(sb);
    printf("  Owner Id:                 %s                       (UID = %d)\n", owner, sb.st_uid);
    // free(owner);

    group = get_group_name(sb);
    printf("  Group Id:                 %s                       (GID = %d)\n", group, sb.st_gid);
    // free(group);

    printf("  Preferred I/O block size: %ld bytes\n", (long)sb.st_blksize);
    printf("  File size:                %lld bytes\n", (long long)sb.st_size);

    printf("  Blocks allocated:         %lld\n", (long long)sb.st_blocks);
    atime = format_time(&sb.st_atime);
    printf("  Last file access:         %s\n", atime);
    free(atime);
    mtime = format_time(&sb.st_mtime);
    printf("  Last file modification:   %s\n", mtime);
    free(mtime);
    ctime = format_time(&sb.st_ctime);
    printf("  Last status change:       %s\n", ctime);
    free(ctime);
}

char *get_mode(struct stat sb)
{
    char *text = malloc(11);
    strcpy(text, "----------");

    switch (sb.st_mode & S_IFMT)
    {
    case S_IFDIR:
        text[0] = 'd';
        break;
    case S_IFIFO:
        text[0] = 'p';
        break;
    case S_IFLNK:
        text[0] = 'l';
        break;
    case S_IFSOCK:
        text[0] = 's';
        break;
    }

    if (sb.st_mode & 0b100)
    {
        // world can read
        text[1] = 'r';
    }
    if (sb.st_mode & 0b10)
    {
        // world can write
        text[2] = 'w';
    }
    if (sb.st_mode & 0b1)
    {
        // world can execute
        text[3] = 'x';
    }
    if (sb.st_mode & 0b100000)
    {
        // group can read
        text[4] = 'r';
    }
    if (sb.st_mode & 0b10000)
    {
        // group can write
        text[5] = 'w';
    }
    if (sb.st_mode & 0b1000)
    {
        // group can execute
        text[6] = 'x';
    }
    if (sb.st_mode & 0b100000000)
    {
        // owner can read
        text[7] = 'r';
    }
    if (sb.st_mode & 0b10000000)
    {
        // owner can write
        text[8] = 'w';
    }
    if (sb.st_mode & 0b1000000)
    {
        // owner can execute
        text[9] = 'x';
    }

    return text;
}

char *get_owner_name(struct stat sb)
{
    struct passwd *owner = getpwuid(sb.st_uid);
    return owner->pw_name;
}
char *get_group_name(struct stat sb)
{
    struct group *g = getgrgid(sb.st_gid);
    return g->gr_name;
}

char *extract_filename(char *name)
{
    int i;
    if (name == NULL)
    {
        return NULL;
    }

    if (name[0] == '.' && name[1] == '/')
    {
        return name + 2;
    }

    for (i = strlen(name); i > 0; i--)
    {
        if (name[i] == '/' || name[i] == '\\')
        {
            return name + (i + 1);
        }
    }

    return name;
}

char *format_time(__time_t *time)
{
    struct tm *local;
    char *buf = malloc(1024);
    memset(buf, 0, 1024);
    local = localtime(time);
    strftime(buf, 1023, "%Y-%m-%d %H:%M:%S %z (%Z) %a (local)", local);
    return buf;
}
