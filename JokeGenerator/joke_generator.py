#!/usr/bin/env python3
"""
Random Joke Generator
Uses JokeAPI (https://jokeapi.dev) to fetch random jokes
"""

import requests
import json
from typing import Dict, Optional
import time

class JokeGenerator:
    """A class to generate random jokes from external API"""
    
    BASE_URL = "https://jokeapi.dev/api/joke"
    
    # Available joke categories
    CATEGORIES = {
        "Any": "Any",
        "General": "General",
        "Programming": "Programming",
        "Knock-Knock": "Knock-Knock",
        "Dad": "Dad",
        "Spooky": "Spooky",
        "Christmas": "Christmas"
    }
    
    def __init__(self, timeout: int = 5):
        """
        Initialize the Joke Generator
        
        Args:
            timeout (int): API request timeout in seconds
        """
        self.timeout = timeout
        self.session = requests.Session()
    
    def get_random_joke(self, category: str = "Any", safe_mode: bool = True) -> Optional[Dict]:
        """
        Fetch a random joke from the API
        
        Args:
            category (str): Joke category (Any, General, Programming, Knock-Knock, Dad, Spooky, Christmas)
            safe_mode (bool): If True, excludes offensive jokes
            
        Returns:
            Dict: Joke data or None if request fails
            
        Example:
            {
                "type": "single",
                "joke": "Why don't scientists trust atoms? Because they make up everything!",
                "category": "General"
            }
        """
        try:
            # Validate category
            if category not in self.CATEGORIES:
                category = "Any"
            
            # Build URL parameters
            params = {
                "safe-mode": "true" if safe_mode else "false"
            }
            
            # Build the full URL
            url = f"{self.BASE_URL}/{category}"
            
            # Make the request
            response = self.session.get(url, params=params, timeout=self.timeout)
            response.raise_for_status()
            
            # Parse and return the joke
            joke_data = response.json()
            return joke_data
            
        except requests.exceptions.Timeout:
            print("❌ Error: Request timed out. Please try again.")
            return None
        except requests.exceptions.ConnectionError:
            print("❌ Error: Could not connect to the API. Check your internet connection.")
            return None
        except requests.exceptions.HTTPError as e:
            print(f"❌ HTTP Error: {e.response.status_code}")
            return None
        except json.JSONDecodeError:
            print("❌ Error: Invalid response from API")
            return None
        except Exception as e:
            print(f"❌ Unexpected error: {str(e)}")
            return None
    
    def format_joke(self, joke_data: Dict) -> str:
        """
        Format joke data for display
        
        Args:
            joke_data (Dict): Joke data from API
            
        Returns:
            str: Formatted joke text
        """
        if not joke_data:
            return "No joke available"
        
        if joke_data.get("error"):
            return f"Error: {joke_data.get('message', 'Unknown error')}"
        
        joke_type = joke_data.get("type", "")
        category = joke_data.get("category", "Unknown")
        
        if joke_type == "single":
            # Single-line joke
            joke_text = joke_data.get("joke", "")
            return f"[{category}] {joke_text}"
        
        elif joke_type == "twopart":
            # Two-part joke (setup and delivery)
            setup = joke_data.get("setup", "")
            delivery = joke_data.get("delivery", "")
            return f"[{category}]\n{setup}\n\n{delivery}"
        
        return "Invalid joke format"
    
    def print_joke(self, joke_data: Dict) -> None:
        """
        Print a formatted joke to console
        
        Args:
            joke_data (Dict): Joke data from API
        """
        formatted = self.format_joke(joke_data)
        print("\n" + "=" * 60)
        print(formatted)
        print("=" * 60 + "\n")
    
    def get_random_jokes(self, count: int = 5, category: str = "Any", 
                        delay: float = 1.0, safe_mode: bool = True) -> list:
        """
        Fetch multiple random jokes
        
        Args:
            count (int): Number of jokes to fetch
            category (str): Joke category
            delay (float): Delay between requests in seconds
            safe_mode (bool): If True, excludes offensive jokes
            
        Returns:
            list: List of joke data
        """
        jokes = []
        for i in range(count):
            print(f"Fetching joke {i+1}/{count}...", end="\r")
            joke = self.get_random_joke(category, safe_mode)
            if joke:
                jokes.append(joke)
            if i < count - 1:
                time.sleep(delay)  # Respect API rate limits
        print(" " * 50, end="\r")  # Clear the line
        return jokes
    
    def close(self) -> None:
        """Close the session"""
        self.session.close()
    
    @staticmethod
    def list_categories() -> Dict:
        """
        List all available joke categories
        
        Returns:
            Dict: Available categories
        """
        return JokeGenerator.CATEGORIES


def main():
    """Main function to demonstrate the Joke Generator"""
    
    print("\n" + "=" * 60)
    print("🤣 RANDOM JOKE GENERATOR 🤣")
    print("=" * 60 + "\n")
    
    # Initialize the generator
    generator = JokeGenerator()
    
    try:
        while True:
            print("Options:")
            print("1. Get a random joke")
            print("2. Get joke by category")
            print("3. Get multiple jokes")
            print("4. List categories")
            print("5. Exit")
            print()
            
            choice = input("Choose an option (1-5): ").strip()
            
            if choice == "1":
                print("\n🔄 Fetching a random joke...")
                joke = generator.get_random_joke(safe_mode=True)
                generator.print_joke(joke)
            
            elif choice == "2":
                print("\nAvailable categories:")
                for i, cat in enumerate(generator.list_categories().keys(), 1):
                    print(f"  {i}. {cat}")
                
                cat_input = input("\nChoose category: ").strip()
                categories_list = list(generator.list_categories().keys())
                
                try:
                    category = categories_list[int(cat_input) - 1]
                    print(f"\n🔄 Fetching a {category} joke...")
                    joke = generator.get_random_joke(category=category, safe_mode=True)
                    generator.print_joke(joke)
                except (ValueError, IndexError):
                    print("❌ Invalid category selection")
            
            elif choice == "3":
                try:
                    count = int(input("How many jokes? (1-10): ").strip())
                    count = max(1, min(10, count))  # Limit to 1-10
                    
                    print("\n🔄 Fetching jokes...")
                    jokes = generator.get_random_jokes(count=count, safe_mode=True)
                    
                    for i, joke in enumerate(jokes, 1):
                        print(f"\n📝 Joke {i}:")
                        generator.print_joke(joke)
                
                except ValueError:
                    print("❌ Invalid input")
            
            elif choice == "4":
                print("\n📋 Available Joke Categories:")
                for cat in generator.list_categories().keys():
                    print(f"  • {cat}")
                print()
            
            elif choice == "5":
                print("\n👋 Goodbye! Thanks for the laughs!")
                break
            
            else:
                print("❌ Invalid option. Please try again.\n")
    
    finally:
        generator.close()


if __name__ == "__main__":
    main()
