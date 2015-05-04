import unittest
import pytc
from identityResolve import *
import random

DBNAME = 'people.db'

class TestDDB(unittest.TestCase):

    def testLittleEndianConversion(self):
        self.assertEquals(int(0x0d0c0b0a), convertToUnicode('\x0a\x0b\x0c\x0d'))

    #identityResolve tool uses recursive calls to build a Person object.
    #This test matches the process iteratively. 
    def test10RandomKeyDynamically(self):
        db = pytc.BDB(DBNAME, pytc.BDBOREADER)
        keys = []
        for key in db.iterkeys():
            keys.append(key)

        testnum = 0
        while testnum<10:
            randomKey = random.choice(keys)
            p = buildPerson(db, randomKey, Person(randomKey))
            
            key = randomKey
            value = ""
            while(1):
                value = db[key]

                if value[0] == 'n':
                    self.assertEquals(value[1:] in p.anonIds, True)
                    key = value[1:]

                elif value[0] == 'i':
                    break

                else:
                    print 'Found an identifier value starting with neither \"n\" nor \"i\"\n'
                    break
            
            self.assertEquals(value[1:], p.internalId)
            testnum += 1

if __name__ == '__main__':
        unittest.main()
