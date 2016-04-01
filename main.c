/**
 Peter McCloskey
 CS 0449 Project 1 Id3Tag reader/editor
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct id3tag
{
    char tagID[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
    char comment[28];
    unsigned char separator;
    unsigned char track;
    unsigned char genre;
} id3tag;

void printId3Tag(id3tag *tag) {
    printf("Title: %s\n", tag->title);
    printf("Artist: %s\n", tag->artist);
    printf("Album: %s\n", tag->album);
    printf("Year: %s\n", tag->year);
    printf("Comment: %s\n", tag->comment);
    printf("Track: %d\n", tag->track);
    printf("Genre: %d\n", tag->genre);
    printf("--------\n");
    
}

int main (int argc, char **argv)
{
    int i;
    int tag_exists = 1;
    char field[10];
    char track[30];
    struct id3tag tag;
    FILE *f = fopen (argv[1], "rb+");
    if (f == NULL)
    {
        printf("Failed to open fail.\n");
        printf("Exiting program\n");
        return 0;
    } else {
        if (fseek(f, -128, SEEK_END) == -1)
        {
            printf("fseek failed\n");
        } else {
            if (fread(&tag, sizeof(tag), 1, f) != 1){
                printf("Failed to read tag\n");
            }
        }
        if (strncmp(tag.tagID, "TAG", 3) != 0) { // Check to see if the file has a tag
            printf("No ID3TAG present.\n");
            tag_exists = 0; //
        }
        // file has a tag and and there are no edits
        if (argc == 2 && tag_exists == 1) {
            printId3Tag(&tag);
        } else if (argc >= 2) { // edit or create new tag
            // create new tag
            if (tag_exists == 0){
                // Fill tag with 0s
                strncpy(tag.title, "0", 30);
                strncpy(tag.artist, "0", 30);
                strncpy(tag.album, "0", 30);
                strncpy(tag.year, "0", 4);
                strncpy(tag.comment, "0", 28);
                tag.track = 0;
                tag.genre = 0;
            }
            // consume all command line arguments and edit the specified fields
            for (i = 2; i < argc; i+=2){
                strcpy(field, argv[i]);
                if (strcmp(field, "-title") == 0)
                {   strncpy(tag.title, argv[i+1], 30);
                } else if (strcmp(field, "-artist") == 0){
                    strncpy(tag.artist, argv[i+1], 30);
                } else if (strcmp(field, "-album") == 0){
                    strncpy(tag.album, argv[i+1], 30);
                } else if (strcmp(field, "-year") == 0){
                    strncpy(tag.year, argv[i+1], 4);
                } else if (strcmp(field, "-comment") == 0){
                    strncpy(tag.comment, argv[i+1], 28);
                } else if (strcmp(field, "-track") == 0){
                    strncpy(track, argv[i+1], 3);
                    if (atoi(track) >= 0){
                        tag.track = atoi(track);
                    }
                } else if (strcmp(field, "-year") == 0){
                    strncpy(tag.year, argv[i+1], 4);
                } 
                    // The user made an error inputting the field or value
                    printf ("Improper formating.\nFailed to edit or create an ID3Tag\n");
                    return 0;
                }
            }
            
            // Determine where to write the tag to
            if (tag_exists == 0) {
                // We have created a new tag so append it at the end of the file
                fseek(f, 0, SEEK_END);
            } else {
                // We need to edit an existing tag so seek to beginning of tag
                fseek(f, -128, SEEK_END);
            }
            // Write the tag to the file
            fwrite(&tag, sizeof(tag), 1, f);
            if (tag_exists == 0) {
                printf("Created a new tag with the following values:\n");
                printId3Tag(&tag);
            } else {
                printf("Changed the existing tag to a tag with the following values:\n");
                printId3Tag(&tag);
            }
        }
    fclose(f);
    }
        return 0;
}



