#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa16cf51b, "module_layout" },
	{ 0x7c32d0f0, "printk" },
	{ 0xcf8d2e6f, "kmem_cache_alloc_trace" },
	{ 0xb44e414c, "kmalloc_caches" },
	{ 0x1f6e82a4, "__register_chrdev" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x28cc25db, "arm_copy_from_user" },
	{ 0x5f754e5a, "memset" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0xb8f3845c, "gpiod_set_raw_value" },
	{ 0x23d07d00, "gpiod_unexport" },
	{ 0x37a0cba, "kfree" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x16e53ba9, "gpiod_export" },
	{ 0xd260b89d, "gpiod_direction_output_raw" },
	{ 0x28eca8fe, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0xf4fa543b, "arm_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xb81960ca, "snprintf" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

