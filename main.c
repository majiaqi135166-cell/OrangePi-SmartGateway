#include"monitor.h"

int keep_running = 1;
	int main(int argc,char *argv[])
	{
		signal(SIGINT, handle_sigint);
		struct SystemState state = { 0 };
		//load_config(&state);
		if (argc == 3)
		{
			printf("检测到命令行参数，正在覆盖配置文件...\n");
			state.hot = atof(argv[1]);
			state.warm = atof(argv[2]);
		}
		else
		{
			load_config(&state);
		}
		int server_fd = socket(AF_INET, SOCK_STREAM, 0);
		int opt = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(8080);

		bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
		listen(server_fd, 5);
		state.server_fd = server_fd;
		
		pthread_t web_tid;
		if (pthread_create(&web_tid, NULL, web_server_thread, &state)!=0)
		{
			perror("线程创建失败\n");
			return 1;
		}

		printf("--- 系统启动 ---\n");
		printf("Web 监控页: http://10.195.211.83:8080\n");

		while (keep_running)
		{
			state.current_temp = log_temperature();
			control_hardware(&state);
			write_log(&state, "Running");

			sleep(2);
		}
		close(server_fd);
		printf("系统已安全退出\n");
		return 0;
	}

