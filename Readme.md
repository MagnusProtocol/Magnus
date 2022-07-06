# Magnus

## What is this?

Magnus is supposed to be a very fast p2p file sharing protocol, whilst being relatively simple and easy to use.

## How does it work?

Magnus aims to be simple and has a relatively simple architecture:

    Step 1: Read the data from a file
    Step 2: Compress said data (Default algorithm is ZSTD)
    Step 3: take the compressed data and encrypt it (via RSA, diffie-hellman key exchange)
    Step 4: Slice the data into parts via our custom function
    Step 5: Calculate the hashes of each part and store them in a file (optionally, magnus might encrypt it)
    Step 6: Establish a TCP connection with the the reciever and send the file with the hashes
    Step 7: As soon as a response is recieved, the ports are confirmed to be open 
        and a test UDP packet has successfully travelled without losses, magnus starts the file transfer.
    Step 8: As soon as the file transfer completes, magnus verifies all the individual parts by comparing the hashes of the parts.
    Step 9: If some hashes do not match or some parts are missing, a TCP request is sent 
        to the server with the index of the parts that were lost/tampered with.
    Step 10: After the transfer has successfully completed with 0 losses, magnus closes the TCP socket and the transfer is complete.

## FAQ
    
    Q. "This is a scam! I just read the word server! it was supposed to be P2P?!!!"
    A. Each "Node" is a server and a client at the same time, we will also add support for multiple downloads and uploads at the same time.

    Q. Why not just use TCP?
    A. Because TCP is much slower than UDP and according to our theories, transfer via UDP and verification via UDP should be a faster alternative.
    
    Q. Why even try to do this? why not use something that exists and make it better?
    A. Because we do not want to continue someone's legacy and our goal is to make software that is better than what's out there.
        Whether we succeed at it or not is a story for another day and more importantly not something we care much about.
    
    Q. Will this have a GUI?
    A. Yes, as soon as we have a working CLI version of magnus, we will start working on a GUI interface for Linux and MacOS. They will have separate repos.

    Q. What if XYZ happens?
    A. We'll fix it.
    
    Q. "What if my IP gets stolen?"
    A. Your fault. This software is licenced under the GPLv3 licence and how you use it is none of our concern.

## Goals

Magnus doesn't attempt to be a bloated piece of crap that only a few wizards can use, therefore it's goals are very simple and minimalist.

    - Being extremely fast
    - Being relatively secure
    - Being end-to-end encrypted
    - Having a cryptocurrency-like accounts system.
    - No need for a server to perform connection brokerage
    - Optional TOR-like routing over the network for transfers marked "safe"
    - Eventually having a nice and simple GTK based GUI
    - Somehow anonymizing users
    - Having a nice community

If all that sounds like something you want, help me make it by opening issues and PRs!
In the odd case that someone _does_ open a PR, please make sure that the code follows the WebKit style guide and the snake case naming convention whilst being legible, C++ code following the previous codebase's convention.
