#ifndef _TLB_SIMPLE_H
#define _TLB_SIMPLE_H

/*
 * Simple, preemptible TLB flush implementation.
 */

# include <linux/config.h>
# include <linux/swap.h>
# include <asm/pgalloc.h>
# include <asm/tlbflush.h>

struct mmu_gather {
	void *self;
};

/*
 * We store the mm in the tlb pointer itself, so we dont
 * have to allocate anything on tlb-gather:
 */
#define tlb_mm(tlb) ((struct mm_struct *)(tlb))

static inline struct mmu_gather *
tlb_gather_mmu(struct mm_struct *mm, unsigned int full_mm_flush)
{
	return (struct mmu_gather *)mm;
}
static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	free_page_and_swap_cache(page);
}
static inline void tlb_free(struct mmu_gather *tlb)
{
	if (tlb_mm(tlb)->rss)
		tlb_mm(tlb)->rss--;
}

#ifndef __ARCH_HAS_4LEVEL_HACK
# define pud_free_tlb(tlb, pudp) __pud_free_tlb(tlb, pudp)
#endif

# define tlb_remove_tlb_entry __tlb_remove_tlb_entry
# define pmd_free_tlb __pmd_free_tlb
# define pte_free_tlb __pte_free_tlb
# define tlb_migrate_finish(mm) do { } while (0)
# define tlb_is_full_mm(tlb) 1

#define pgd_free_tlb(tlb, pgdp) __pgd_free_tlb(tlb, pgdp)

static inline void
tlb_finish_mmu(struct mmu_gather *tlb, unsigned long start, unsigned long end)
{
	flush_tlb_mm(tlb_mm(tlb));
	check_pgt_cache();
}

#endif
