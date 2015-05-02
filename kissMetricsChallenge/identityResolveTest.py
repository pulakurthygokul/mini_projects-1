import unittest
import pytc
from identityResolve import *
import random

DBNAME = 'people.db'

class TestDDB(unittest.TestCase):

    def checkFirstVal(self, person, db, key):
            firstValue = (db[key])[1:]
            return (firstValue in person.anonIds) or (firstValue == person.internalId)

    def testLittleEndianConversion(self):
        self.assertEquals(int(0x0d0c0b0a), convertToUnicode('\x0a\x0b\x0c\x0d'))

    def testFooAnonId(self):
        key = 'foo'
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        p = buildPerson(db, key, Person(key))
        anonValue = db['foo'] 
        self.assertEquals(p.anonIds[0], anonValue[1:])

    def test10RandomKeys(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        keys = []
        for key in db.iterkeys():
            keys.append(key)
        num = 0
        while num<10:
            randomKey = random.choice(keys)
            p = buildPerson(db, randomKey, Person(randomKey))
            testResult = self.checkFirstVal(p,db,randomKey)
            self.assertEquals(True, testResult)
            num+=1

    def testFirst10Keys(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        num = 0
        for key in db.iterkeys():
            if num>9:
                break
            p = buildPerson(db, key, Person(key))
            testResult = self.checkFirstVal(p, db, key)
            self.assertEquals(True, testResult)
            num+=1

    def test10AnonIdSplicing(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        num = 0
        for key in db.iterkeys():
            if num>9:
                break
            num+=1
            p = buildPerson(db, key, Person(key))
            for anonId in p.anonIds:
                self.assertNotEquals('n', anonId[0])

    def test10internalIdSplicing(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        num = 0
        for key in db.iterkeys():
            if num>9:
                break
            num+=1
            p = buildPerson(db, key, Person(key))
            self.assertNotEquals('i', p.internalId[0])

if __name__ == '__main__':
        unittest.main()
