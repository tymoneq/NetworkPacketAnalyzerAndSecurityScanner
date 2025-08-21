from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy import create_engine
import os
from ..logger.logger import *


Base = declarative_base()

# SQLite (file-based). Replace with PostgreSQL/MySQL if needed
app_dir = os.path.dirname(os.path.abspath(__file__))
db_dir = os.path.join(app_dir, "../../../database")
os.makedirs(db_dir,exist_ok=True)

db_path = os.path.join(db_dir,"database.db")
engine = create_engine(f"sqlite:///{db_path}")

SessionLocal = sessionmaker(bind=engine)


def initDb():
    Base.metadata.create_all(engine)
    writeToLogPy(info, "Database initialized")

def closeDb(session):
    session.close()
