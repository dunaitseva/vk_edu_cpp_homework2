#include "include/matrix.h"

#include <pthread.h>
#include <sys/sysinfo.h>
#include <malloc.h>
#include <unistd.h>

typedef struct {
  matrix_t *src;
  matrix_t *dest;
  size_t start;
  size_t end;
} transpose_routine_args_t;

static void set_status(int *status, int status_code);
static size_t determine_threads_amount();
static void *transpose_routine(void *args_struct);
static int prepare_resources(pthread_t **threads, transpose_routine_args_t **args, size_t threads_amount);
static void free_resources(pthread_t *threads, transpose_routine_args_t *args);

matrix_t *transpose(matrix_t *matrix, int *status) {
  if (matrix == NULL) {
	set_status(status, EPTR);
	return NULL;
  }

  matrix_t *transposed = create_matrix(matrix->cols, matrix->rows, NULL);
  if (transposed == NULL) {
	set_status(status, EALLOC);
	return NULL;
  }

  size_t thread_amount = determine_threads_amount();
  size_t segment_size = matrix->rows / thread_amount + 1;
  pthread_t *threads;
  transpose_routine_args_t *tmp_args;
  prepare_resources(&threads, &tmp_args, thread_amount);

  for (size_t i = 0; i < thread_amount; ++i) {
	tmp_args[i].src = matrix;
	tmp_args[i].dest = transposed;
	tmp_args[i].start = i * segment_size;
	tmp_args[i].end = (i + 1) * segment_size;

	int err = pthread_create(&threads[i], NULL, transpose_routine, &tmp_args[i]);
	if (err) {
	  set_status(status, ETHREAD);
	  return NULL;
	}
  }

  for (size_t i = 0; i < thread_amount; ++i) {
	int err = pthread_join(threads[i], NULL);
	if (err) {
	  free_resources(threads, tmp_args);
	  set_status(status, ETHREAD);
	  return NULL;
	}
  }
  free_resources(threads, tmp_args);

  set_status(status, OK);
  return transposed;
}

static void *transpose_routine(void *args_struct) {
  transpose_routine_args_t *args = (transpose_routine_args_t *)args_struct;
  for (size_t i = args->start; i < args->end && i < args->src->rows; ++i) {
	for (size_t j = 0; j < args->src->cols; ++j) {
	  matrix_set_val(args->dest, matrix_get_val(args->src, i, j, NULL), j, i);
	}
  }

  return args_struct;
}

static void set_status(int *status, int status_code) {
  if (status != NULL) {
	*status = status_code;
  }
}

static size_t determine_threads_amount() {
  return get_nprocs();
//  return 3;
}

static int prepare_resources(pthread_t **threads, transpose_routine_args_t **args, size_t threads_amount) {
  *threads = (pthread_t *)calloc(threads_amount, sizeof(pthread_t));
  if (*threads == NULL) {
	return EALLOC;
  }

  *args = (transpose_routine_args_t *)calloc(threads_amount, sizeof(transpose_routine_args_t));
  if (*args == NULL) {
	return EALLOC;
  }

  return OK;
}

static void free_resources(pthread_t *threads, transpose_routine_args_t *args) {
  free(threads);
  free(args);
}