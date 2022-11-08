/*
PSEUDOCODE for recover


Open memory card
Repeat until end of card:
	Read 512 bytes into a buffer
	If start of a new JPEG
		If first JPEG
			Start writing first file
		Else (if you've already found a jpeg)
			Make sure to close file you've been writing to &
			Open new file
	Else
		If already found JPEG
			Keep writing to it (the next 512 byte block)
Close any remaining files
*/