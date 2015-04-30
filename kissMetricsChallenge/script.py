from tokyocabinet import hash

db = hash.Hash() 
db.open('people.db', hash.HDBOREADER)
