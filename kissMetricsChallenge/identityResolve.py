import pytc
import struct
import numpy as np

class Person:
    def __init__(self, name, internalId=None, anonIds=None):
        self.name = name
        self.internalId = internalId
        self.anonIds = []

def exceptionHandle(e):
    if type(e)==KeyError:
        print '\nException: Identity not found\n'
    else:
        print '\nException: Unknown exception searching for identity\n'

def buildPerson(db, key, person):
    try:
        value = db[key]
    except Exception, e:
        exceptionHandle(e)
        return person
    
    if value[0] == 'n':
        newKey = value[1:]
        person.anonIds.append(newKey)
        return buildPerson(db, newKey, person)

    elif value[0] == 'i':
        person.internalId = value[1:] 
        return person

    else:
        print 'Found an identifier value starting with neither \"n\" nor \"i\"\n'
        return person

def convertToUnicode(string):
    #same result as: return np.fromstring(string, dtype=np.uint32)
    structReturnTuple = struct.unpack('<I', string)
    return structReturnTuple[0]

def outputResults(p):
    print "\nName: " + p.name
    print "Anonymous identifiers: " + str(p.anonIds)
    print "Raw unisigned 32 bit little endian internal identifier: " + repr(p.internalId)
    print "Base 10 value: " + str(convertToUnicode(p.internalId)) + "\n"

if __name__ == "__main__":
    fname = raw_input("Database file path: ")
    db = pytc.BDB(fname, pytc.BDBOREADER)
    while(1):
        name = raw_input("Identity to be resolved (type \'exit()\' to exit): ")
        if name=='exit()':
            break
        p = buildPerson(db, name, Person(name))
        if(p.internalId):
            outputResults(p)
