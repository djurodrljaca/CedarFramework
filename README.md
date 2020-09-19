# Cedar Framework

This library provides the following features:

* querying for specific nodes in JSON
* serialization of data structure to JSON
* deserialization of data structures from JSON

## Querying nodes

Note: The *query module* only works with *JSON Array* or *JSON Object* as the root node.

The *CedarFramework::hasNode()* function checks if a JSON data structure contains a sub-node:

* at index (*JSON Array*)
* with name (*JSON Object*)
* at path (sequence of indexes and/or names)

The *CedarFramework::getNode()* function extracts a sub-node from a JSON data structure:

* at index (*JSON Array*)
* with name (*JSON Object*)
* at path (sequence of indexes and/or names)

## Serialization

The *CedarFramework::serialize()* function serializes a native value to an equivalent *JSON value* and *CedarFramework::deserialize()* function deserializes a *JSON value* to a native value.

## Deserialization

The *CedarFramework::deserializeNode()* and *CedarFramework::deserializeOptionalNode()* convenience functions can be used to deserialize either a mandatory or optional sub-node of a JSON data structure directly instead of first extracting the sub-node and then deserializing it.

## Supported types

The following table shows how the supported native types are serialized to JSON:

| Native type               | JSON representation
| ------------------------- | -------------------
| bool                      | *JSON Boolean*
| *integers*                | Integers that can be stored in a *double* without loss of percision (between -9,007,199,254,740,992 and 9,007,199,254,740,992) are stored as a *JSON Number* all others are stored as a *JSON String*
| *floating point*          | All floating point values are stored as a *Number*
| *strings*                 | All string types are stored a *JSON String*
| QChar                     | *JSON String*
| QByteArray                | Base64 encoded *JSON String*
| QBitArray                 | *JSON Object* with:<br>* "bit_count" (*JSON Number*) <br>* "encoded_bits" (Base64 encoded *JSON String*)
| QDate                     | *JSON String* in ISO 8601 format (yyyy-MM-dd)
| QTime                     | *JSON String* in ISO 8601 format with millisecond percision (HH:mm:ss.sss)
| QDateTime                 | *JSON String* in ISO 8601 format with millisecond percision (yyyy-MM-ddTHH:mm:ss.sssTZ) where *TZ* is:<br>* empty for local time<br>* "Z" for UTC time<br>* "[+\|-]HH:mm" for offset from UTC
| QVariant                  | * serialized according to the actual type in the QVariant value<br>* deserialized as a direct conversion of QJsonValue to QVariant (exact native type of the originally serialized value is not available!)
| QUrl                      | *JSON String*
| QUuid                     | *JSON String*
| QLocale                   | *JSON String* in BCP47 format
| QRegExp                   | *JSON Object* with:<br>* "pattern" (*JSON String*)<br>* "case_sensitive" (*JSON Boolean*)<br>* "syntax" (*JSON String* representation of QRegExp::PatternSyntax)
| QRegularExpression        | *JSON Object* with:<br>* "pattern" (*JSON String*)<br>* "options" (*JSON Array* representation of QRegularExpression::PatternOptions)
| QSize and QSizeF          | *JSON Object* with:<br>* "width" (*JSON Number* or *String*)<br>* "height" (*JSON Number* or *String*)
| QPoint and QPointF        | *JSON Object* with:<br>* "x" (*JSON Number* or *String*)<br>* "y" (*JSON Number* or *String*)
| QLine and QLineF          | *JSON Object* with:<br>* "x1" (*JSON Number* or *String*)<br>* "y1" (*JSON Number* or *String*)<br>* "x2" (*JSON Number* or *String*)<br>* "y2" (*JSON Number* or *String*)
| QRect and QRectF          | *JSON Object* with:<br>* "x" (*JSON Number* or *String*)<br>* "y" (*JSON Number* or *String*)<br>* "width" (*JSON Number* or *String*)<br>* "height" (*JSON Number* or *String*)
| QStringList               | *JSON Array*
| QJsonValue                | *JSON Value*
| QJsonArray                | *JSON Array*
| QJsonObject               | *JSON Object*
| QJsonDocument             | *JSON Value*
| QCborValue                | *JSON Value*
| QCborArray                | *JSON Array*
| QCborMap                  | *JSON Object*
| QCborSimpleType           | *JSON Value*:<br>* QCborSimpleType::False and QCborSimpleType::True (*JSON Boolean*)<br>* QCborSimpleType::Null (*JSON Null*)
| QPair<T1, T2>             | *JSON Object* with:<br>* "first" (serialized *T1* type)<br>* "second" (serialized *T2* type)
| std::pair<T1, T2>         | *JSON Object* with:<br>* "first" (serialized *T1* type)<br>* "second" (serialized *T2* type)
| QList\<T>                 | *JSON Array*
| std::list\<T>             | *JSON Array*
| QVector\<T>               | *JSON Array*
| std::vector\<T>           | *JSON Array*
| QSet\<T>                  | *JSON Array*
| QMap<K, V>                | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Value*
| std::map<K, V>            | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Value*
| QHash<K, V>               | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Value*
| std::unordered_map<K, V>  | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Value*
| QMultiMap<K, V>           | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Array*
| QMultiHash<K, V>          | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Array*

Note: in maps the key must be of a native type that is serializable to either *JSON String" or *JSON Number"!

## Custom types

The user can create serialization and deserialization functions for native types not supported by the library by creating the specialized *CedarFramework::serialize()* amd *CedarFramework::deserialize()* functions.
