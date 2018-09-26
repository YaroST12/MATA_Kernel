#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/buildvariant.h>

static int cmdline_proc_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", saved_command_line);
	return 0;
}

static int cmdline_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cmdline_proc_show, NULL);
}

static const struct file_operations cmdline_proc_fops = {
	.open		= cmdline_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

bool is_userdebug(void)
{
	/* Defined in buildvariant.h */
	bool userdebug =
		(strstr(saved_command_line, "buildvariant=userdebug") ||
		strstr(saved_command_line, "buildvariant=eng"));

	if (userdebug)
		pr_warn("build variant userdebug or eng");
	else
		pr_warn("build variant user");

	return userdebug;
}

static int __init proc_cmdline_init(void)
{
	proc_create("cmdline", 0, NULL, &cmdline_proc_fops);
	return 0;
}
fs_initcall(proc_cmdline_init);
