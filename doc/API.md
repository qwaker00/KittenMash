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
  	"image" : <url of image>,
  	"rating" : <float>,
  	"gameCount" : <int>
  },
  ...    
]
```

---
### Create vote
**POST** /vote/

Response
  * **Code**: 302
  * **Content-Type**: application/json
  * **Location**: /vote/\<vote_id\>
  * **Body**

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
    "leftImage": "Left item identificator",
    "rightImage": "Right item identificator",
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
      * Success - 302
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
