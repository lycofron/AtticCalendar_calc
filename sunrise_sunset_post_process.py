#!/usr/bin/env python3
from zoneinfo import ZoneInfo
from datetime import timezone
import pandas as pd


def jd2localDT(juliandate):
    t = pd.to_datetime(juliandate, origin='julian', unit='D')
    return t.to_pydatetime().replace(tzinfo=timezone.utc).astimezone(ZoneInfo('Europe/Athens')).strftime("%d/%m/%Y %H:%M:%S")


def readResultsCsvFile(fname):
    csvData = list()
    with open(fname) as f:
        for line in f.readlines():
            csvData.append([float(x) for x in line.split()])
    return csvData


if __name__ == "__main__":
    sunriseData = readResultsCsvFile("results/sunrise_sunset.700.4.csv")
    for i in sunriseData:
        ss = jd2localDT(i[0])
        sr = jd2localDT(i[1])
        print("Sunset: %s Sunrise: %s" % (ss, sr))
