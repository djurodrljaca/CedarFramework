# Cedar Framework

This library provides the following features:

* querying for specific nodes in JSON
* serialization of data structure to JSON
* deserialization of data structures from JSON


## Build and deployment

For building the following requirements are needed:

* CMake v3.5 or later
* C++14 compliant compiler
* Qt5 framework (only Core and Test modules)

If the library needs to be deployed to a custom location you need to set the ```CMAKE_INSTALL_PREFIX``` variable or if you need to use a custom Qt installation you need to set the ```CMAKE_PREFIX_PATH``` variable.

Example:

```
$ cmake -DCMAKE_INSTALL_PREFIX=path/to/install/dir -DCMAKE_PREFIX_PATH=path/to/qt/dir path/to/source/dir
```

Then build and deploy the library:

```
$ cmake --build . --target install
```


## Usage

### CMake Integration

To use this module in a CMake project you need to build and deploy it. Then just add it as a package to your project's CMakeLists.txt:

```
find_package(CedarFramework REQUIRED)
```

And link it to your target:

```
target_link_libraries(target_name PUBLIC CedarFramework::CedarFramework)
```


### Querying nodes

The *CedarFramework::hasNode()* function checks if a JSON data structure contains a sub-node:

* at index (*JSON Array*)
* with name (*JSON Object*)
* at path (sequence of indexes and/or names)

The *CedarFramework::getNode()* function extracts a sub-node from a JSON data structure:

* at index (*JSON Array*)
* with name (*JSON Object*)
* at path (sequence of indexes and/or names)

**Note: The *query module* only works with *JSON Array* or *JSON Object* as the root node.**


### Serialization

The *CedarFramework::serialize()* function serializes a native value to an equivalent *JSON value* and *CedarFramework::deserialize()* function deserializes a *JSON value* to a native value.


### Deserialization

The *CedarFramework::deserializeNode()* and *CedarFramework::deserializeOptionalNode()* convenience functions can be used to deserialize either a mandatory or optional sub-node of a JSON data structure directly instead of first extracting the sub-node and then deserializing it.


### Supported types

The following table shows how the supported native types are serialized to JSON:

| Native type               | JSON representation
| ------------------------- | -------------------
| bool                      | *JSON Boolean*
| *integers*                | Integers that can be stored in a *double* without loss of precision (between -9,007,199,254,740,992 and 9,007,199,254,740,992) are stored as a *JSON Number* all others are stored as a *JSON String*
| *floating point*          | All floating point values are stored as a *Number*
| *strings*                 | All string types are stored a *JSON String*
| QChar                     | *JSON String*
| QByteArray                | Base64 encoded *JSON String*
| QBitArray                 | *JSON Object* with:<br><ul><li>"bit_count" (*JSON Number*)</li><li>"encoded_bits" (Base64 encoded *JSON String*)</li></ul>
| QDate                     | *JSON String* in ISO 8601 format (yyyy-MM-dd)
| QTime                     | *JSON String* in ISO 8601 format with millisecond precision (HH:mm:ss.sss)
| QDateTime                 | *JSON String* in ISO 8601 format with millisecond precision (yyyy-MM-ddTHH:mm:ss.sssTZ) where *TZ* is:<br><ul><li>empty for local time</li><li>"Z" for UTC time</li><li>"[+\|-]HH:mm" for offset from UTC</li></ul>
| QVariant                  | Serialized according to the actual type in the QVariant value
| QUrl                      | *JSON String*
| QUuid                     | *JSON String*
| QLocale                   | *JSON String* in BCP47 format
| QRegExp                   | *JSON Object* with:<br><ul><li>"pattern" (*JSON String*)</li><li>"case_sensitive" (*JSON Boolean*)</li><li>"syntax" (*JSON String* representation of QRegExp::PatternSyntax)</li></ul>
| QRegularExpression        | *JSON Object* with:<br><ul><li>"pattern" (*JSON String*)</li><li>"options" (*JSON Array* representation of QRegularExpression::PatternOptions)</li></ul>
| QSize<br>QSizeF           | *JSON Object* with:<br><ul><li>"width" (*JSON Number* or *String*)</li><li>"height" (*JSON Number* or *String*)</li></ul>
| QPoint<br>QPointF         | *JSON Object* with:<br><ul><li>"x" (*JSON Number* or *String*)</li><li>"y" (*JSON Number* or *String*)</li></ul>
| QLine<br>QLineF           | *JSON Object* with:<br><ul><li>"x1" (*JSON Number* or *String*)</li><li>"y1" (*JSON Number* or *String*)</li><li>"x2" (*JSON Number* or *String*)</li><li>"y2" (*JSON Number* or *String*)</li></ul>
| QRect<br>QRectF           | *JSON Object* with:<br><ul><li>"x" (*JSON Number* or *String*)</li><li>"y" (*JSON Number* or *String*)</li><li>"width" (*JSON Number* or *String*)</li><li>"height" (*JSON Number* or *String*)</li></ul>
| QJsonValue                | *JSON Value*
| QJsonArray                | *JSON Array*
| QJsonObject               | *JSON Object*
| QJsonDocument             | *JSON Value*
| QCborValue                | *JSON Value*
| QCborArray                | *JSON Array*
| QCborMap                  | *JSON Object*
| QCborSimpleType           | *JSON Value*:<br><ul><li>QCborSimpleType::False and QCborSimpleType::True (*JSON Boolean*)</li><li>QCborSimpleType::Null (*JSON Null*)</li></ul>
| QPair<T1,T2><br>std::pair<T1,T2> | *JSON Object* with:<br><ul><li>"first" (serialized *T1* type)</li><li>"second" (serialized *T2* type)</li></ul>
| QStringList<br>QList\<T><br>std::list\<T><br>QVector\<T><br>std::vector\<T><br>QSet\<T> | *JSON Array*
| QMap<K,V><br>std::map<K,V><br>QHash<K,V><br>std::unordered_map<K,V> | *JSON Object* with the key serialized to *JSON String* and the value serialized to *JSON Value*
| QMultiMap<K,V><br>QMultiHash<K,V> | *JSON Object* with the key serialized to *JSON String* and the values serialized to *JSON Array*

**Note: a *JSON Value* can be deserialized to a QVariant value only as a direct conversion of QJsonValue to QVariant because the exact native type of the originally serialized value is not available!**

**Note: in maps the key must be of a native type that is serializable to either *JSON String* or *JSON Number*!**


### Custom types

The user can create serialization and deserialization functions for native types not supported by the library by creating the specialized *CedarFramework::serialize()* and *CedarFramework::deserialize()* functions.
