/* Quickscope - a software oscilloscope
 * Copyright (C) 2012-2014  Lance Arsenault
 * GNU General Public License version 3
 */

#include <limits.h>
#include <sys/wait.h>
#include <libgen.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <gtk/gtk.h>


struct Run
{
  int run_count;
  GtkWidget *button;
  char *program;
};


// Add the Path of this program to the environment
// variable PATH
static void setPathEnv(void)
{
  // TODO: /proc/self/exe is specific to Linux
  //
  char *path = realpath("/proc/self/exe" , NULL);
  if(!path) return;
  if(!path[0]) goto cleanup;

  // Remove filename and last '/'
  char *end;
  end = path;
  while(*(++end));
  --end;
  while(*end != '/' && end != path)
    *(end--) = '\0';
  if(end != path && *end == '/')
    *end = '\0';

  char *envPath;
  envPath = getenv("PATH");
  if(!envPath) goto cleanup;

  char *newPath;
#ifdef QS_TESTS
  if(strcmp(".","."))
    // built in other dir
    newPath = g_strdup_printf("%s:%s:%s", "./tests", path, envPath);
  else
    // built in top source dir
    newPath = g_strdup_printf("%s:%s", path, envPath);
#else
  newPath = g_strdup_printf("%s:%s", path, envPath);
#endif

  setenv("PATH", newPath, 1);
  fprintf(stderr, "set PATH=\"%s\"\n", newPath);

  g_free(newPath);

cleanup:

  free(path);
}

static
void setLabelString(struct Run *run)
{
  GtkLabel *label;
  label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(run->button)));
  char *color;
  switch(run->run_count)
  {
    case 0:
      color = "#000";
      break;
    default:
      color = "#f55";
      break;
  }

  // We make a label that has the same length for all labels.

  char *t1, *t2;
  t1 = g_strdup_printf("%s "
      "                                                  "
      "                                                  ",
      run->program);
  t2 = g_strdup_printf("%100.100s", t1);
  g_free(t1);
  t1 = g_markup_printf_escaped("%s", t2);
  g_free(t2);
  t2 = g_strdup_printf(
          "<span font_family=\"monospace\" "
          ">%4d)  </span>"
          "<span font_family=\"monospace\" "
          "font_weight=\"bold\" "
          "color=\"%s\">%s</span>",
          run->run_count++, color, t1);
  g_free(t1);

  gtk_label_set_markup(label, t2);

  g_free(t2);
}

static bool run_cb(GtkWidget *button, struct Run *run)
{
  fprintf(stderr, "running:");
  fprintf(stderr, " %s", run->program);
  fprintf(stderr, "\n");

  setLabelString(run);

  // pipe command to bash

  int fd[2];

  if(pipe(fd) == -1)
  {
    fprintf(stderr, "pipe() failed: %s\n",
        strerror(errno));
    return true;
  }

  if(fork() == 0)
  {
    // child
    close(fd[1]);
    // make stdin be the reading end of the pipe
    if(dup2(fd[0], 0) == -1)
    {
      fprintf(stderr, "dup2(%d, 0) failed: %s\n",
          fd[0], strerror(errno));
      exit(1);
    }
    // bash will read the pipe
    execl("/bin/bash", "bash", NULL);
    fprintf(stderr, "failed to execute: %s in bash pipeline\n", run->program);
    exit(1);
  }

  // The parent write the bash command to the pipe
  if(write(fd[1], run->program, strlen(run->program)) != strlen(run->program))
  {
    fprintf(stderr, "write(%d, \"%s\", %zu) failed: %s\n",
        fd[1], run->program, strlen(run->program), strerror(errno));
    exit(1);
  }
  if(write(fd[1], "\n", 2) != 2)
  {
    fprintf(stderr, "write(%d, \"%s\", %d) failed: %s\n",
        fd[1], "\\n", 2, strerror(errno));
    exit(1);
  }
  close(fd[0]);
  close(fd[1]);
  return true;
}

static void addButton(const char *args, GtkContainer *vbox)
{
  struct Run *run;
  run = g_malloc0(sizeof(*run));
  run->button = gtk_button_new_with_label(" ");
  run->program = (char *) args;
  g_signal_connect(run->button, "clicked", G_CALLBACK(run_cb), (void *) run);
  gtk_box_pack_start(GTK_BOX(vbox), run->button, false, false, 0);
  gtk_widget_show(run->button);
  setLabelString(run);
}

static
bool ecb_keyPress(GtkWidget *w, GdkEvent *e, void* data)
{
  switch(e->key.keyval)
  {
    case GDK_KEY_Q:
    case GDK_KEY_q:
    case GDK_KEY_Escape:
      gtk_main_quit();
      return true;
      break;
  }
  return false;
}

static void makeWidgets(const char *title, const char *const programs[])
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *button;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), title);
  g_signal_connect(G_OBJECT(window), "key-press-event",
      G_CALLBACK(ecb_keyPress), NULL);
  g_signal_connect(window, "delete_event", G_CALLBACK(gtk_main_quit), NULL);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

  button = gtk_button_new_with_label("Quit");
  g_signal_connect(button, "clicked", G_CALLBACK(gtk_main_quit), NULL);
  gtk_box_pack_start(GTK_BOX(vbox), button, false, false, 0);
  gtk_widget_show(button);
  
  gtk_container_add(GTK_CONTAINER(window), vbox);

  const char *const *program;
  program = programs;

  while(*program)
  {
    addButton(*program, GTK_CONTAINER(vbox));
    ++program;
  }

  gtk_widget_show(vbox);

  gtk_widget_show(window);
}

#define DSO_SYMBOL   "run"

static int
usage(char *argv0)
{
    fprintf(stderr,
        "Usage: %s [--help|-h]\n"
        "\n"
        "   A simple button launcher program.\n"
        "\n"
        "   The list of programs to launch are read from stdin.\n"
        "\n"
        "   This launcher does not manage the\n"
        " programs that it launches, so quiting this launcher will\n"
        " not terminate all the launched programs.  If you terminate\n"
        " this launcher by signaling it from a shell, the shell may\n"
        " also terminate all the launched programs too, but that's\n"
        " just the shell doing its thing and not this launcher.\n"
        " This launcher program is intentionally simple and is not\n"
        " a graphical shell.  It's just a program launcher.\n"
        "\n"
        "\n"
        ,
        basename(argv0));
    return 1;
}

static
const char *const *getFileList(char *filename)
{
    FILE *file = 0;
    if(!filename || !filename[0] || (
            filename[0] == '-' && !filename[1]))
        file = stdin;
    else if(!(file = fopen(filename, "r")))
    {
        fprintf(stderr, "fopen(\"%s\", 'r') failed: %s\n",
            filename, strerror(errno));
        return NULL;
    }

    char *line = NULL;
    size_t num_progs = 0;
    char **programs = NULL;
    size_t line_length = 0;
    while(getline(&line, &line_length, file) != -1)
    {
        if(line && line[0] && line[0] != '#')
        {
            size_t l;
            while((l = strlen(line)) && (line[strlen(line) - 1] == '\n' ||
            line[strlen(line) - 1] == '\r'))
            line[strlen(line) - 1] = '\0';
            if(line[0])
            {
                programs = realloc(programs, sizeof(char *) * (++num_progs + 1));
                programs[num_progs - 1] = strdup(line);
                programs[num_progs] = NULL;
            }
        }
    }
    if(line)
        free(line);

    if(!programs)
        fprintf(stderr,
            "No programs listed in file: %s\n",
            filename);

    if(file != stdin)
        fclose(file);

    return (const char *const *) programs;
}


void sigChildCatcher(int sig)
{
  int status;
  wait(&status);
}

int main(int argc, char **argv)
{
    const char *const *programs;
    char *title;
    title = basename(argv[0]);

    if(argc == 2 && (
                !strcmp("-h", argv[1]) ||
                !strcmp("--help", argv[1])))
        return usage(argv[0]);

    // Make run list only one way.
    if(!(programs = getFileList(NULL)))
        return usage(argv[0]);

  setPathEnv();
  gtk_init(&argc, &argv);
  makeWidgets(title, programs);
  signal(SIGCHLD, sigChildCatcher);
  gtk_main();

  return 0;
}
