import itertools
from collections import Counter
from math import floor
# from statistics import fmean

dataKeys = ["true", "topocentric", "apparent"]


class DateInfo(object):
    __splitAt = 16

    def __init__(self, dateFragment):
        self.julianDate = float(dateFragment[0:self.__splitAt])
        self.dateLiteral = dateFragment[self.__splitAt+1:].strip()

    def __repr__(self):
        return "%s (%f)" % (self.dateLiteral, self.julianDate)


class DateInfoCouple(object):
    __splitAt = 37

    def __init__(self, dateFragment):
        self.preciseDate = DateInfo(dateFragment[0:self.__splitAt])
        self.atticDay = DateInfo(dateFragment[self.__splitAt+1:])

    def __repr__(self):
        return "%s - %s" % (self.preciseDate, self.atticDay)


class SoLunarEventData(object):
    __splitAt = 75

    def __init__(self, dateFragment):
        self.trueDate = DateInfoCouple(dateFragment[0:self.__splitAt])
        self.topoDate = DateInfoCouple(dateFragment[self.__splitAt+1:self.__splitAt*2+1])
        self.apparentDate = DateInfoCouple(dateFragment[self.__splitAt*2+2:])

    def __repr__(self):
        return "%s\n%s\n%s\n" % (self.trueDate, self.topoDate, self.apparentDate)


def readResultsCsvFile(fname):
    csvData = list()
    with open(fname) as f:
        for line in f.readlines():
            csvData.append(SoLunarEventData(line))
    return csvData


def pairwise(iterable):
    "s -> (s0, s1), (s1, s2), (s2, s3), ..."
    a, b = itertools.tee(iterable)
    next(b, None)
    return zip(a, b)


def distance2unit(f):
    frac = f % 1
    return min(frac, 1-frac)


def setMonthLengths(months, days):
    '''This function will allocate days into months in Meton's style, that is, omitting every nth day from months of 30 days.'''
    initialDays = months * 30
    quota = initialDays / (initialDays - days)
    monthDays = [item for sublist in [[x] * 30 for x in range(months)] for item in sublist]
    toRemove = [floor(quota*(x+1))+1 for x in list(range(floor(initialDays / quota)))]
    toRemove.reverse()
    for x in toRemove:
        if x < len(monthDays):
            del monthDays[x]
    return Counter(monthDays)


def trueDatePeriodicStats(dataset, period):
    # Let's calculate the cumulative error over 6000 years
    dataSubSet = dataset[::period]  # choose every PERIODth new moon
    periodIterations = len(dataSubSet)-1  # size of list minus one are the iterations of the period that will have occurred
    # How many days have passed from start of first period to the end of the last period
    daysPast = round(dataSubSet[-1].trueDate.atticDay.julianDate - dataSubSet[0].trueDate.atticDay.julianDate)
    periodDuration = round(daysPast/periodIterations)  # Approximation of period duration, in days
    cumulatedError = abs(periodDuration * periodIterations - daysPast)
    # if cumulatedError == 0:
    #     monthsOff
    #     for i in range(period):  # try all possible starts
    #         dataSubSet = dataset[i::period]
    #         sum([abs((y.trueDate.atticDay.julianDate - x.trueDate.atticDay.julianDate) - periodDuration)
    #             for x, y in zip(dataSubSet, dataSubSet[1:])])
    #         startPoint = dataset[i]
    #         endPoint = dataset[i+period]
    #         statList.append(endPoint.trueDate.preciseDate.julianDate - startPoint.trueDate.preciseDate.julianDate)
    return [period, cumulatedError, daysPast/periodIterations]


if __name__ == "__main__":
    # solsticeData = readResultsCsvFile(r"results/solstices_athens.csv")
    lunarData = readResultsCsvFile(r"results/new_moon_athens.csv")
    periodStats = list()
    for i in range(39, 3760):
        # if (not i % 10):
        #     print(i)
        x = trueDatePeriodicStats(lunarData, i)
        periodStats.append(x)

    for i in sorted(periodStats, key=lambda e: e[1]):
        print(i)

    # firstNewMoon = lunarData[0].trueDate.preciseDate.julianDate
    # deltaMin = 1.0
    # minData = None
    # for x in lunarData[1:3760]:
    #     deltaCurrent = x.trueDate.preciseDate.julianDate - firstNewMoon
    #     if (deltaCurrent % 1 < deltaMin):
    #         deltaMin = deltaCurrent % 1
    #         minData = x
    # print(minData.trueDate.preciseDate.dateLiteral, minData.trueDate.preciseDate.julianDate - firstNewMoon,
    #       (minData.trueDate.preciseDate.julianDate - firstNewMoon) / 29.53)

    # for cur, nxt in pairwise(lunarData):
    #     if nxt.trueDate.preciseDate.julianDate - cur.trueDate.preciseDate.julianDate < 29:
    #         print(cur.trueDate.preciseDate.julianDate, nxt.trueDate.preciseDate.dateLiteral,
    #               nxt.trueDate.preciseDate.julianDate - cur.trueDate.preciseDate.julianDate)
