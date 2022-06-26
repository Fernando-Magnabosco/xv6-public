void scheduler(void)
{
    struct proc *p;
    struct cpu *c = mycpu();

    c->proc = 0;

    for (;;)
    {
        // Enable interrupts on this processor.
        sti();

        // Loop over process table looking for process to run.
        acquire(&ptable.lock);

        for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
        {
            if (p->state != RUNNABLE)
                continue;

            // Switch to chosen process.  It is the process's job
            // to release ptable.lock and then reacquire it
            // before jumping back to us.
            c->proc = p;

            switchuvm(p);

            p->state = RUNNING;

            swtch(&(c->scheduler), p->context);

            switchkvm();

            // Process is done running for now.
            // It should have changed its p->state before coming back.
            c->proc = 0;
        }

        release(&ptable.lock);
    }
}
// --------------------------------------------------------------------------------
void scheduler(void)
{

    struct proc *p;
    struct cpu *c = mycpu();

    int lastRunnable = -1;

    acquire(&ptable.lock);

    int i;
    for (i = 0; i < NPROC; i++)
    {

        if (ptable.proc[i].state != RUNNABLE)
        {
            ptable.acc[i] = -1;
            continue;
        }

        if (lastRunnable == -1)
            ptable.acc[i] = ptable.proc[i].noTickets;
        else
            ptable.acc[i] = ptable.acc[lastRunnable] + ptable.proc[i].noTickets;

        lastRunnable = i;
    }

    release(&ptable.lock);

    c->proc = 0;

    for (;;)
    {
        // Enable interrupts on this processor.
        sti();

        // Loop over process table looking for process to run.
        acquire(&ptable.lock);

        int drawn = rand_r(ptable.proc[lastRunnable].pid) % ptable.acc[lastRunnable];

        for (i = 0; i < NPROC; i++)
        {

            if (ptable.acc[i] == -1)
                continue;
            if (drawn < ptable.acc[i])
            {
                p = &ptable.proc[i];
                break;
            }
        }

        if (i == NPROC)
        {
            panic("No process found");
            continue;
        }
        // updateaccremove(i);

        c->proc = p;
        switchuvm(p);
        p->state = RUNNING;

        swtch(&(c->scheduler), p->context);
        switchkvm();

        // Process is done running for now.
        // It should have changed its p->state before coming back.
        c->proc = 0;

        release(&ptable.lock);
    }
}