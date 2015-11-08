# Kitten Mash

## Overview
Web-service to choose best images of a kitten in a world using side-by-side comparations.
Two images of kittens given to user at time, and he should choose which one is better. System builds Elo rating based on all users' votes.
User free to add new images of a kittens to service.

## API

### List items' rating top
**GET** /top

Response
 * **Code**: 200
  * **Content-Type**: application/json
  * **Body**
```
[
  {
  	"image" : <url of image>
  	"rating" : <float>
  },
  ...    
]
```

---
### Create vote
**POST** /vote/

Response
  * **Code**: 201
  * **Content-Type**: application/json
  * **Location**: /vote/\<vote_id\>
  * **Body**
```
{
  "item_id_left": "Left item identificator",
  "item_id_right": "Right item identificator",
}
```

---
### Get vote info
**GET** /vote/\<vote_id\>

Response
  * **Code**: 
      * Success - 200
      * No such vote - 404
  * **Content-Type**: application/json
  * **Body**
```
{
    "item_id_left": "Left item identificator",
    "item_id_right": "Right item identificator",
}
```

---
### Get vote result
**GET** /vote/\<vote_id\>/result

Response
  * **Code**: 200
  * **Content-Type**: text/plain
  * **Body**
```
left|right
```

---
### Set vote result
**PUT** /vote/\<vote_id\>/result

Request
  * **Content-Type**: text/plain
  * **Request body**
```
left|right
```

Response
  * **Code**: 
      * Success - 200
      * Incorrect vote_id or body - 400

---
### Add new image
**POST** /image/

Request
  * **Content-Type**: image/*
  * **Body**: \<item image in some known image format\>

Response
  * **Code**: 
      * Success - 201
      * There is no kitten on image - 400
  * **Location**: /image/\<item_id\>

---
### Get image by item id
**GET** /image/\<item_id\>

Response
  * **Code**: 
      * Success - 200
      * Not such item - 404
  * **Content-Type**: image/*
  * **Body**: \<item image in some known image format\>
