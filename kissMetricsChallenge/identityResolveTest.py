import unittest
import pytc
from identityResolve import *
import random

DBNAME = 'people.db'

class TestDDB(unittest.TestCase):

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


    def test10KnownKeys(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        num = 0
        for key in db.iterkeys():
            if num>9:
                break
            else:
                num+=1
                p = buildPerson(db, key, Person(key))
                firstValue = (db[key])[1:]
                testResult = (firstValue in p.anonIds) or (firstValue == p.internalId)
                self.assertEquals(True, testResult)

    def test10AnonIdSplicing(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        num = 0
        for key in db.iterkeys():
            if num>9:
                break
            else:
                num+=1
                p = buildPerson(db, key, Person(key))
                for anonId in p.anonIds:
                    self.assertNotEquals('n', anonId[0])

    def test10interalIdSplicing(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        num = 0
        for key in db.iterkeys():
            if num>9:
                break
            else:
                num+=1
                p = buildPerson(db, key, Person(key))
                self.assertNotEquals('i', p.internalId[0])

if __name__ == '__main__':
        unittest.main()
