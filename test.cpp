#include <b.h>

void start_server(int port) {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[sizeof(Person)] = {0};
    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 绑定socket到端口
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))
             < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听是否有客户端连接
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // 接受客户端连接
    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // 读取从客户端发送的数据
    read(client_socket, buffer, sizeof(buffer));
    Person* person = reinterpret_cast<Person*>(buffer);
    std::cout << "Received ID: " << person->id << ", Age: " << person->age << std::endl;

    // 关闭连接
    close(client_socket);
    close(server_fd);
}

int main() {
    int port = 8080;
    start_server(port);
    return 0;