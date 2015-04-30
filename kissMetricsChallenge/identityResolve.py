import pytc
import struct

class Person:
    def __init__(self, name, internalId=None, anonId=None):
        self.name = name
        self.internalId = internalId
        self.anonId = []

def exceptionHandle(e):
    if type(e)==KeyError:
        print 'Identity not found'
    else:
        print 'Unknown exception searching for identity'

def buildPerson(db, key, person):
    try:
        value = db[key]
    except Exception, e:
        exceptionHandle(e)
        return person
    
    if value[0] == 'n':
        newKey = value[1:]
        person.anonId.append(newKey)
        return buildPerson(db, newKey, person)

    elif value[0] == 'i':
        person.internalId = value[1:] 
        return person

    else:
        print 'Found an identifier value starting with neither \"n\" nor \"i\"'
        return person

def convertToUnicode(string):
    s = []
    for char in string: 
        unicodeString = str((ord(char)))
        reverse = unicodeString[::-1] 
        s[:0] = reverse 
    return ('').join(s) 

def prompt():
    fname = raw_input("Database file path: ")
    identity = raw_input("Identity to be resolved: ")
    return fname, identity


if __name__ == "__main__":
    #fname = 'people.db'
    #name = 'foo'
    fname, name = prompt()
    db = pytc.BDB(fname, pytc.BDBOREADER)
    p = buildPerson(db, name, Person(name))
    print convertToUnicode(p.internalId)
