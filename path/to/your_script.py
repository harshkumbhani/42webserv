import sys

def main():
    # Get the arguments passed to the script
    args = sys.argv[1:]
    
    # Process the arguments and generate a response
    response = f"Received arguments: {', '.join(args)}"
    
    # Print the response (this will be sent back to the CGI script)
    print(response)

if __name__ == "__main__":
    main()
