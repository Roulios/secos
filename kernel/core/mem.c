/** 
 * Setup the OS memory
 * Bedrossian Paul 
 * 18/11/26
 */

#include <debug.h>
#include <info.h>
#include <segmem.h>

/**
 * MEMO
 * SS - Section selector
 * 
 */

/**
 * @brief Helper function to setup a segement descriptor (FLAT)
 * @param sd segment descriptor to setup
 * @param type type of segment
 * @param dpl ring must be included between 0 and 3
 */
void __seg_descr_setup(seg_desc_t *sd, uint8_t type, uint8_t dpl)
{
    sd->limit_1 = 0xFFFF;
    sd->limit_2 = 0xF;
    sd->base_1 = 0;
    sd->base_2 = 0;
    sd->base_3 = 0;
    sd->type = type;
    sd->s = 1;
    sd->dpl = dpl;
    sd->p = 1;
    sd->avl = 0;
    sd->l =0 ;
    sd->d = 1;
    sd->g = 1;
}

/**
 * @brief Helper function to enable protected mode in cr0
 */ 
void __enable_cr0_pe() 
{
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 1; // Set PE (Protection Enable) bit
    asm volatile ("mov %0, %%cr0" : : "r" (cr0) : "memory");
}

/**
 * @brief Helper function to load the new selectors
 */
void __load_selectors()
{
    set_cs(gdt_krn_seg_sel(1));
    set_ds(gdt_krn_seg_sel(2));
    set_ss(gdt_krn_seg_sel(2));
    set_es(gdt_krn_seg_sel(2));
}

/**
 * @brief Switch segmentation from real mode to protected mode
 * 
 */
void protected_mode()
{
    // GDT - Global descriptors table 
    static gdt_reg_t gdtr_protected_mode;
    // SD - Segment descriptors
    static seg_desc_t seg_desc[5];
    seg_desc[0] = (seg_desc_t){0}; // First seg_desc is the null desc
    __seg_descr_setup(&seg_desc[1], SEG_DESC_CODE_X, 0);
    __seg_descr_setup(&seg_desc[2], SEG_DESC_DATA_RW, 0);
    __seg_descr_setup(&seg_desc[3], SEG_DESC_CODE_X, 3);
    __seg_descr_setup(&seg_desc[4], SEG_DESC_DATA_RW, 3);

    gdtr_protected_mode.desc = seg_desc;
    gdtr_protected_mode.limit = sizeof(seg_desc) - 1;

    // Set gdtr
    set_gdtr(gdtr_protected_mode);   

    // Change the cr0 register to enable protected mode
    __enable_cr0_pe();

    // Load new selectors
    __load_selectors();

    debug("[MEMORY] Segmentation - Protected mode enabled \n");
}