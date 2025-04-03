/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tester.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naorakot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 13:57:32 by naorakot          #+#    #+#             */
/*   Updated: 2025/03/22 14:11:03 by naorakot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	char	buf[7];
	int fd = open("/tmp", O_TMPFILE | O_RDWR | O_EXCL, 00777);
	if (fd == -1)
	{
		perror("open");
		return (0);
	}
	write(fd, "hello\n", 6);
	rewind(fd);
	if (read(fd, buf, 6) == 0)
		printf("reading nothing");
	else
		write(1, buf, 6);
	close(fd);
	return (0);
}
//my problem seems to be that my fd will move on forward regardless of the thing i try to do with it :: as such, i can only write in it, and will read picking back up from where i last wrote (EOF). would need to be able to rewind the read stream, but htat's not allowed. pipes working great so far, see no need for anything else
