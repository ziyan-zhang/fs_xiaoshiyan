#include <stdio.h>      //printf
#include <fcntl.h>      //O_REONLY
#include <sys/types.h>  //ssize_t
#include <unistd.h>     //close

#define BUF_LEN 4096

int main(int argc, char* argv[]) {
    int src_fd, dest_fd;
    char data_buf[BUF_LEN];
    ssize_t read_count = 0;
    ssize_t write_count = 0;
    int ret = 0;

    src_fd = open(argv[1], O_RDONLY);
    if (-1 == src_fd) {
        printf("open src file error!");
        goto ERR_OUT;
    }

    //打开目标文件，目标文件以写模式打开
    dest_fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (-1 == dest_fd) {
        printf("open dest file error!");
        goto OUT;
    }

    //拷贝数据
    while ((read_count = read(src_fd, &data_buf, BUF_LEN)) > 0) {
        ssize_t data_remain = read_count;
        while (data_remain > 0) {
            write_count = write(dest_fd, &data_buf, data_remain);
            if (write_count < 0) {
                printf("copy data error!");
                goto FIN_OUT;
            }
            data_remain -= write_count;
        }
    }

    //任何读取或写入失败都要提示用户
    if (read_count < 0 || write_count < 0) {
        printf("copy data error!");
    }
FIN_OUT:
    //关闭文件
    close(dest_fd);
OUT:
    close(src_fd);
ERR_OUT:
    return(0);
}