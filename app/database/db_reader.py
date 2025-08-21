import sqlite3
import pandas as pd
import os
import sys

class DatabaseReader:
    def __init__(self, db_path):
        """Initialize the database reader with a path to the .db file"""
        self.db_path = db_path
        self.connection = None
        
    def connect(self):
        """Connect to the SQLite database"""
        try:
            if not os.path.exists(self.db_path):
                raise FileNotFoundError(f"Database file '{self.db_path}' not found")
            
            self.connection = sqlite3.connect(self.db_path)
            print(f"Successfully connected to: {self.db_path}")
            return True
        except Exception as e:
            print(f"Error connecting to database: {e}")
            return False
    
    def get_tables(self):
        """Get list of all tables in the database"""
        try:
            cursor = self.connection.cursor()
            cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
            tables = [row[0] for row in cursor.fetchall()]
            return tables
        except Exception as e:
            print(f"Error getting tables: {e}")
            return []
    
    def get_table_info(self, table_name):
        """Get column information for a specific table"""
        try:
            cursor = self.connection.cursor()
            cursor.execute(f"PRAGMA table_info({table_name});")
            columns = cursor.fetchall()
            return columns
        except Exception as e:
            print(f"Error getting table info: {e}")
            return []
    
    def query_table(self, table_name, limit=10):
        """Query a table and return results as a pandas DataFrame"""
        try:
            query = f"SELECT * FROM {table_name} LIMIT {limit};"
            df = pd.read_sql_query(query, self.connection)
            return df
        except Exception as e:
            print(f"Error querying table: {e}")
            return pd.DataFrame()
    
    def execute_custom_query(self, query):
        """Execute a custom SQL query"""
        try:
            df = pd.read_sql_query(query, self.connection)
            return df
        except Exception as e:
            print(f"Error executing query: {e}")
            return pd.DataFrame()
    
    def get_row_count(self, table_name):
        """Get the number of rows in a table"""
        try:
            cursor = self.connection.cursor()
            cursor.execute(f"SELECT COUNT(*) FROM {table_name};")
            count = cursor.fetchone()[0]
            return count
        except Exception as e:
            print(f"Error getting row count: {e}")
            return 0
    
    def explore_database(self):
        """Print a summary of the database structure"""
        print("\n" + "="*50)
        print("DATABASE EXPLORATION")
        print("="*50)
        
        tables = self.get_tables()
        if not tables:
            print("No tables found in the database.")
            return
        
        print(f"Found {len(tables)} table(s):")
        
        for table in tables:
            print(f"\n📋 Table: {table}")
            row_count = self.get_row_count(table)
            print(f"   Rows: {row_count}")
            
            columns = self.get_table_info(table)
            print("   Columns:")
            for col in columns:
                col_id, name, data_type, not_null, default, pk = col
                pk_indicator = " (PRIMARY KEY)" if pk else ""
                null_indicator = " NOT NULL" if not_null else ""
                print(f"     - {name} ({data_type}){pk_indicator}{null_indicator}")
    
    def close(self):
        """Close the database connection"""
        if self.connection:
            self.connection.close()
            print("Database connection closed.")

def main():
    print("SQLite Database Reader")
    print("="*30)
    
    # Get database file path from user
    if len(sys.argv) > 1:
        db_path = sys.argv[1]
    else:
        db_path = input("Enter the path to your .db file: ").strip()
    
    # Create database reader instance
    db_reader = DatabaseReader(db_path)
    
    # Connect to database
    if not db_reader.connect():
        return
    
    try:
        while True:
            print("\n" + "="*30)
            print("OPTIONS:")
            print("1. Explore database structure")
            print("2. View table data")
            print("3. Execute custom query")
            print("4. Export table to CSV")
            print("5. Exit")
            
            choice = input("\nEnter your choice (1-5): ").strip()
            
            if choice == '1':
                db_reader.explore_database()
                
            elif choice == '2':
                tables = db_reader.get_tables()
                if not tables:
                    print("No tables found.")
                    continue
                
                print("\nAvailable tables:")
                for i, table in enumerate(tables, 1):
                    print(f"{i}. {table}")
                
                try:
                    table_choice = int(input("Select table number: ")) - 1
                    if 0 <= table_choice < len(tables):
                        table_name = tables[table_choice]
                        limit = input("Number of rows to display (default 10): ").strip()
                        limit = int(limit) if limit.isdigit() else 10
                        
                        df = db_reader.query_table(table_name, limit)
                        if not df.empty:
                            print(f"\n📊 First {limit} rows from '{table_name}':")
                            print(df.to_string(index=False))
                        else:
                            print("No data found or error occurred.")
                    else:
                        print("Invalid table selection.")
                except ValueError:
                    print("Please enter a valid number.")
                    
            elif choice == '3':
                query = input("\nEnter your SQL query: ").strip()
                if query:
                    df = db_reader.execute_custom_query(query)
                    if not df.empty:
                        print("\n📊 Query Results:")
                        print(df.to_string(index=False))
                    else:
                        print("No results or error occurred.")
                        
            elif choice == '4':
                tables = db_reader.get_tables()
                if not tables:
                    print("No tables found.")
                    continue
                
                print("\nAvailable tables:")
                for i, table in enumerate(tables, 1):
                    print(f"{i}. {table}")
                
                try:
                    table_choice = int(input("Select table number: ")) - 1
                    if 0 <= table_choice < len(tables):
                        table_name = tables[table_choice]
                        df = db_reader.query_table(table_name, limit=None)
                        
                        if not df.empty:
                            filename = f"{table_name}.csv"
                            df.to_csv(filename, index=False)
                            print(f"✅ Table '{table_name}' exported to '{filename}'")
                        else:
                            print("No data to export.")
                    else:
                        print("Invalid table selection.")
                except ValueError:
                    print("Please enter a valid number.")
                    
            elif choice == '5':
                break
                
            else:
                print("Invalid choice. Please select 1-5.")
                
    except KeyboardInterrupt:
        print("\n\nExiting...")
    finally:
        db_reader.close()

if __name__ == "__main__":
    main()