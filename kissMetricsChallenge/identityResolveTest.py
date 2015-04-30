import unittest
import pytc
from identityResolve import *

DBNAME = 'people.db'

class TestHDB(unittest.TestCase):

    def test10KnownKeys(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        knownKeys = []
        num = 0 
        for key in db.iterkeys():
            if num>9:
                break
            else:
                num+=1
                p = buildPerson(db, key, Person(key))

if __name__ == '__main__':
        unittest.main()
