int store(int arg)
{
  message m

  if (_syscall(PM_PROC_NR, 49, &m) > 0) 
    printf("%d\n", m.m_type);
}
