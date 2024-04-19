/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kgriset <kgriset@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 17:38:44 by kgriset           #+#    #+#             */
/*   Updated: 2024/04/19 08:46:38 by kgriset          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_receiving;

void	sig_handler(int signum, siginfo_t *info, void *context)
{
	static int	bit_read;

	(void)info;
	(void)context;
	g_receiving = 0;
	if (signum == SIGUSR2)
		++bit_read;
	else if (signum == SIGUSR1)
		ft_printf("%d bytes received\n", bit_read / 8);
}

int	char_to_bin(char c, int pid)
{
	int	iteration;
	int	bit_index;

	iteration = 0;
	bit_index = 7;
	while (bit_index >= 0)
	{
		g_receiving = 1;
		iteration = 0;
		if ((c >> bit_index) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (g_receiving)
		{
			if (iteration == 42)
				return (ft_printf("Server not responding...aborting\n"),
					exit(0), ERROR);
			++iteration;
			usleep(42);
		}
		--bit_index;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	int					pid;
	size_t				byte_index;
	int					status;
	struct sigaction	sa;

	status = SUCCESS;
	byte_index = 0;
	if (argc == 1)
		return (ft_printf("Usage: ./client <pid> <string>\n"), ERROR);
	pid = ft_atoi_safe(argv[1], &status);
	if (argc != 3 || !status || !pid)
		return (ft_printf("Usage: ./client <pid> <string>\n"), ERROR);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = sig_handler;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (argv[2][byte_index])
		char_to_bin(argv[2][byte_index++], pid);
	char_to_bin(0, pid);
	return (SUCCESS);
}
