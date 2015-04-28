#include "utils.h"

static int direct_redirect(int from, int to) {
	ssize_t read_bytes = 0;

	do {
		read_bytes = splice(from, NULL, to, NULL, CHUNK_SIZE, SPLICE_F_MOVE|SPLICE_F_MORE);
		if (read_bytes < 0) {
			perror(__func__);
			return -errno;
		}
	} while (read_bytes > 0);

	return 0;
}

static int piped_redirect(int from, int to) {
	ssize_t read_bytes = 0;
	int pipes[2];
	int r = 0;

	if (pipe(pipes) < 0) {
		r = -errno;
		goto exit;
	}

	do {
		read_bytes = splice(from, NULL, pipes[1], NULL, CHUNK_SIZE, SPLICE_F_MOVE|SPLICE_F_MORE);
		if (read_bytes < 0) {
			perror(__func__);
			r = -errno;
			goto exit_pipes;
		} else if (read_bytes > 0) {

			read_bytes = splice(pipes[0], NULL, to, NULL, (size_t)read_bytes, SPLICE_F_MOVE|SPLICE_F_MORE);
			if (read_bytes < 0) {
				perror(__func__);
				r = -errno;
				goto exit_pipes;
			}
		}
	} while (read_bytes > 0);

exit_pipes:
	close(pipes[0]);
	close(pipes[1]);

exit:
	return r;
}

int redirect(int from, int to) {
	int r = 0;

	r = direct_redirect(from, to);
	if (r == -EINVAL) {
		return piped_redirect(from, to);
	}

	return r;
}

int redirect_from_file(char *path, int to) {
	_cleanup_close_ int fromfd = -1;

	fromfd = open(path, O_RDONLY|O_CLOEXEC);
	if (fromfd < 0) {
		perror(__func__);
		return -errno;
	}

	return redirect(fromfd, to);
}

int redirect_to_file(int from, char *path) {
	_cleanup_close_ int tofd = -1;

	tofd = open(path, O_CREAT|O_WRONLY|O_CLOEXEC, S_IRUSR|S_IWUSR);
	if (tofd < 0) {
		perror(__func__);
		return -errno;
	}

	return redirect(from, tofd);
}
