#include "types.h"

static void gdt_set_entry(u32 entry, u32 base, u32 limit, u8 access, u8 granularity);
void gdt_finalize(void);

struct gdt_desc
{
	u16 size;
	u32 address;
} __attribute__((packed));

struct gdt_entry
{
	u16 limit_low;
	u16 base_low;
	u8 base_mid;
	u8 access;
	u8 granularity;
	u8 base_high;
};

struct gdt_entry entries[3];
struct gdt_desc desc;

void gdt_init(void)
{
	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	desc.address = (u32)&entries;
	desc.size = sizeof(struct gdt_entry) * 3 - 1;

	gdt_finalize();
}

static void gdt_set_entry(u32 entry, u32 base, u32 limit, u8 access, u8 granularity)
{
	entries[entry].base_low = base & 0xFFFF;
	entries[entry].base_mid = (base >> 16) & 0xFF;
	entries[entry].base_high = (base >> 24) & 0xFF;

	entries[entry].limit_low = limit & 0xFFFF;

	entries[entry].granularity = (limit >> 16) & 0x0F;
	entries[entry].granularity |= granularity & 0xF0;

	entries[entry].access = access;
}
