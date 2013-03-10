#include <stdio.h>

/* test program to demonstrate use of interrupts */
/* Jiri Gaisler, Gaisler Research, 2001          */

extern void *catch_interrupt(void func(), int irq);

unsigned long *lreg = (unsigned long *) 0x80000000;

#define LEON3

#ifdef LEON3
#define ILEVEL 0x000
#define ICLEAR 0x20c
#define IPEND 0x204
#define IMASK  0x240
#define IFORCE 0x208
#else
#define ICLEAR 0x9c
#define IMASK  0x90
#define IFORCE 0x98
#endif

unsigned long *timer = (unsigned long *) 0x80000300;

#define TCTRL 0x08
#define T1CTRL 0x18
#define T1VAL 0x10
#define LOADT1 0x14
#define SCALVA 0x00
#define SCALOA 0x04

unsigned long *lcd = (unsigned long *) 0x80000400;

int n = 0;

enable_irq (int irq) 
{

	lreg[ICLEAR/4] = (1 << irq);	// clear any pending irq
	lreg[IMASK/4] |= (1 << irq);	// unmaks irq
}

disable_irq (int irq) { lreg[IMASK/4] &= ~(1 << irq); }	// mask irq

force_irq (int irq) { lreg[IFORCE/4] = (1 << irq); }	// force irq

/* NOTE: NEVER put printf() or other stdio routines in interrupt handlers,
   they are not re-entrant. This (bad) example is just a demo */

void irqhandler(int irq)
{
  n++;
  printf("n = %d\n", n);
  
  if(n<5) lcd[0] = 0x38;
  else if(n<10&&n>5){
    lcd[0] = 0x0C;
    lcd[0] = 0x06;
  }
  else{
    //lcd[0] = 0x01;
    //lcd[0] = 0x80;
    lcd[0] = 0x200 | 'A';
    lcd[0] = 0x249;
    lcd[0] = 0x243;
    lcd[0] = 0x24B;
    lcd[0] = 0x259;
  }
  

}


main()
{
  /*
	catch_interrupt(irqhandler, 10);
	catch_interrupt(irqhandler, 11);
	catch_interrupt(irqhandler, 12);
	enable_irq(10);
	enable_irq(11);
	enable_irq(12);
	force_irq(10);
	force_irq(11);
	force_irq(12);
	force_irq(10);
	lreg[IFORCE/4] = (7 << 10); // force irq 10, 11 & 12 simultaneously
	*/
  
  catch_interrupt(irqhandler,8);
  enable_irq(8);
  
  timer[TCTRL/4] = 0x200;
  timer[SCALOA/4] = 0x2FA;
  timer[SCALVA/4] = 0x2FA;
  
  timer[T1CTRL/4] = 0x0B;
  timer[LOADT1/4] = 0xFFFF;
  timer[T1VAL/4] = 0xFFFF;
  
  
while (1){
}
  
}



