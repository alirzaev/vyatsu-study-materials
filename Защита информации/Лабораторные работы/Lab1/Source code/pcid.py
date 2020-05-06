def pcid() -> str:
    import cpuid
    from functools import reduce

    regs = cpuid.CPUID()(1)

    return str(reduce(lambda x, y: x ^ y, [regs[0], regs[2], regs[3]]))
