import itertools


def readResultsCsvFile(fname):
    csvData = list()
    splitAtHalf = 16
    splitAtFull = 38
    with open(fname) as f:
        for line in f.readlines():
            csvData.append([(float(line[i:i+splitAtHalf]), line[i+splitAtHalf+1:i+splitAtFull].strip())
                            for i in [x for x in range(0, 227, splitAtFull)]])
    return csvData


def pairwise(iterable):
    "s -> (s0, s1), (s1, s2), (s2, s3), ..."
    a, b = itertools.tee(iterable)
    next(b, None)
    return zip(a, b)


if __name__ == "__main__":
    solsticeData = readResultsCsvFile(r"results/solstices_athens.csv")
    metonSample = solsticeData[::19]
    for cur, nxt in pairwise(metonSample):
        print(cur[0], nxt[0], nxt[0][0] - cur[0][0])
