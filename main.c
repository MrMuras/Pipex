/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amurawsk <amurawsk@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 22:40:31 by amurawsk          #+#    #+#             */
/*   Updated: 2023/04/24 01:35:58 by amurawsk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_cmd(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*temp;
	int		i;

	i = 0;
	while (ft_strncmp("PATH", *envp, 4))
		envp++;
	paths = ft_split(*envp + 5, ':');
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = 0;
	while (paths[i])
		free(paths[i++]);
	free(paths);
	return (0);
}

void	execute_function(char *argv, char **envp)
{
	char	**cmds;
	char	*path;
	int		i;

	i = 0;
	cmds = ft_split(argv, ' ');
	path = find_cmd(cmds[0], envp);
	if (!path)
	{
		while (cmds[i])
			free(cmds[i++]);
		free(cmds);
		my_error("Path not found");
	}
	else if (execve(path, cmds, envp) == -1)
		my_error("Exceeve error");
}

void	child1(char **argv, char **envp, int *rura)
{
	int	infile;

	infile = open(argv[1], O_RDONLY, 0644);
	if (infile < 0)
		my_error("Infile");
	dup2(rura[1], STDOUT_FILENO);
	dup2(infile, STDIN_FILENO);
	close(rura[0]);
	execute_function(argv[2], envp);
}

void	child2(char **argv, char **envp, int *rura)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		my_error("Outfile");
	dup2(rura[0], STDIN_FILENO);
	dup2(outfile, STDOUT_FILENO);
	close(rura[0]);
	execute_function(argv[3], envp);
}

int	main(int argc, char **av, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;
	int		rura[2];

	if (argc != 5)
		my_error("argc != 5");
	if (pipe(rura) == -1)
		my_error("err pipe");
	pid1 = fork();
	if (pid1 == -1)
		my_error("forking1");
	if (pid1 == 0)
		child1(av, envp, rura);
	pid2 = fork();
	if (pid2 == -1)
		my_error("forking2");
	if (pid2 == 0)
		child2(av, envp, rura);
	close(rura[0]);
	close(rura[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
