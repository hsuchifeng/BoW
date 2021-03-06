// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: imgret.proto

#ifndef PROTOBUF_imgret_2eproto__INCLUDED
#define PROTOBUF_imgret_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace dmir {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_imgret_2eproto();
void protobuf_AssignDesc_imgret_2eproto();
void protobuf_ShutdownFile_imgret_2eproto();

class Request;
class Request_Color;
class Result;

// ===================================================================

class Request_Color : public ::google::protobuf::Message {
 public:
  Request_Color();
  virtual ~Request_Color();

  Request_Color(const Request_Color& from);

  inline Request_Color& operator=(const Request_Color& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Request_Color& default_instance();

  void Swap(Request_Color* other);

  // implements Message ----------------------------------------------

  Request_Color* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Request_Color& from);
  void MergeFrom(const Request_Color& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 r = 1;
  inline bool has_r() const;
  inline void clear_r();
  static const int kRFieldNumber = 1;
  inline ::google::protobuf::int32 r() const;
  inline void set_r(::google::protobuf::int32 value);

  // required int32 g = 2;
  inline bool has_g() const;
  inline void clear_g();
  static const int kGFieldNumber = 2;
  inline ::google::protobuf::int32 g() const;
  inline void set_g(::google::protobuf::int32 value);

  // required int32 b = 3;
  inline bool has_b() const;
  inline void clear_b();
  static const int kBFieldNumber = 3;
  inline ::google::protobuf::int32 b() const;
  inline void set_b(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:dmir.Request.Color)
 private:
  inline void set_has_r();
  inline void clear_has_r();
  inline void set_has_g();
  inline void clear_has_g();
  inline void set_has_b();
  inline void clear_has_b();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 r_;
  ::google::protobuf::int32 g_;
  ::google::protobuf::int32 b_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_imgret_2eproto();
  friend void protobuf_AssignDesc_imgret_2eproto();
  friend void protobuf_ShutdownFile_imgret_2eproto();

  void InitAsDefaultInstance();
  static Request_Color* default_instance_;
};
// -------------------------------------------------------------------

class Request : public ::google::protobuf::Message {
 public:
  Request();
  virtual ~Request();

  Request(const Request& from);

  inline Request& operator=(const Request& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Request& default_instance();

  void Swap(Request* other);

  // implements Message ----------------------------------------------

  Request* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Request& from);
  void MergeFrom(const Request& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Request_Color Color;

  // accessors -------------------------------------------------------

  // optional string imgPath = 1;
  inline bool has_imgpath() const;
  inline void clear_imgpath();
  static const int kImgPathFieldNumber = 1;
  inline const ::std::string& imgpath() const;
  inline void set_imgpath(const ::std::string& value);
  inline void set_imgpath(const char* value);
  inline void set_imgpath(const char* value, size_t size);
  inline ::std::string* mutable_imgpath();
  inline ::std::string* release_imgpath();
  inline void set_allocated_imgpath(::std::string* imgpath);

  // repeated int32 id = 2;
  inline int id_size() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id(int index) const;
  inline void set_id(int index, ::google::protobuf::int32 value);
  inline void add_id(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      id() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_id();

  // repeated .dmir.Request.Color needColor = 3;
  inline int needcolor_size() const;
  inline void clear_needcolor();
  static const int kNeedColorFieldNumber = 3;
  inline const ::dmir::Request_Color& needcolor(int index) const;
  inline ::dmir::Request_Color* mutable_needcolor(int index);
  inline ::dmir::Request_Color* add_needcolor();
  inline const ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >&
      needcolor() const;
  inline ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >*
      mutable_needcolor();

  // repeated .dmir.Request.Color denyColor = 4;
  inline int denycolor_size() const;
  inline void clear_denycolor();
  static const int kDenyColorFieldNumber = 4;
  inline const ::dmir::Request_Color& denycolor(int index) const;
  inline ::dmir::Request_Color* mutable_denycolor(int index);
  inline ::dmir::Request_Color* add_denycolor();
  inline const ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >&
      denycolor() const;
  inline ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >*
      mutable_denycolor();

  // optional bytes image = 5;
  inline bool has_image() const;
  inline void clear_image();
  static const int kImageFieldNumber = 5;
  inline const ::std::string& image() const;
  inline void set_image(const ::std::string& value);
  inline void set_image(const char* value);
  inline void set_image(const void* value, size_t size);
  inline ::std::string* mutable_image();
  inline ::std::string* release_image();
  inline void set_allocated_image(::std::string* image);

  // @@protoc_insertion_point(class_scope:dmir.Request)
 private:
  inline void set_has_imgpath();
  inline void clear_has_imgpath();
  inline void set_has_image();
  inline void clear_has_image();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* imgpath_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > id_;
  ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color > needcolor_;
  ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color > denycolor_;
  ::std::string* image_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];

  friend void  protobuf_AddDesc_imgret_2eproto();
  friend void protobuf_AssignDesc_imgret_2eproto();
  friend void protobuf_ShutdownFile_imgret_2eproto();

  void InitAsDefaultInstance();
  static Request* default_instance_;
};
// -------------------------------------------------------------------

class Result : public ::google::protobuf::Message {
 public:
  Result();
  virtual ~Result();

  Result(const Result& from);

  inline Result& operator=(const Result& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Result& default_instance();

  void Swap(Result* other);

  // implements Message ----------------------------------------------

  Result* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Result& from);
  void MergeFrom(const Result& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated string imgPath = 1;
  inline int imgpath_size() const;
  inline void clear_imgpath();
  static const int kImgPathFieldNumber = 1;
  inline const ::std::string& imgpath(int index) const;
  inline ::std::string* mutable_imgpath(int index);
  inline void set_imgpath(int index, const ::std::string& value);
  inline void set_imgpath(int index, const char* value);
  inline void set_imgpath(int index, const char* value, size_t size);
  inline ::std::string* add_imgpath();
  inline void add_imgpath(const ::std::string& value);
  inline void add_imgpath(const char* value);
  inline void add_imgpath(const char* value, size_t size);
  inline const ::google::protobuf::RepeatedPtrField< ::std::string>& imgpath() const;
  inline ::google::protobuf::RepeatedPtrField< ::std::string>* mutable_imgpath();

  // repeated int32 id = 2;
  inline int id_size() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id(int index) const;
  inline void set_id(int index, ::google::protobuf::int32 value);
  inline void add_id(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      id() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_id();

  // repeated double precision = 3;
  inline int precision_size() const;
  inline void clear_precision();
  static const int kPrecisionFieldNumber = 3;
  inline double precision(int index) const;
  inline void set_precision(int index, double value);
  inline void add_precision(double value);
  inline const ::google::protobuf::RepeatedField< double >&
      precision() const;
  inline ::google::protobuf::RepeatedField< double >*
      mutable_precision();

  // @@protoc_insertion_point(class_scope:dmir.Result)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::std::string> imgpath_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > id_;
  ::google::protobuf::RepeatedField< double > precision_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_imgret_2eproto();
  friend void protobuf_AssignDesc_imgret_2eproto();
  friend void protobuf_ShutdownFile_imgret_2eproto();

  void InitAsDefaultInstance();
  static Result* default_instance_;
};
// ===================================================================


// ===================================================================

// Request_Color

// required int32 r = 1;
inline bool Request_Color::has_r() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Request_Color::set_has_r() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Request_Color::clear_has_r() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Request_Color::clear_r() {
  r_ = 0;
  clear_has_r();
}
inline ::google::protobuf::int32 Request_Color::r() const {
  return r_;
}
inline void Request_Color::set_r(::google::protobuf::int32 value) {
  set_has_r();
  r_ = value;
}

// required int32 g = 2;
inline bool Request_Color::has_g() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Request_Color::set_has_g() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Request_Color::clear_has_g() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Request_Color::clear_g() {
  g_ = 0;
  clear_has_g();
}
inline ::google::protobuf::int32 Request_Color::g() const {
  return g_;
}
inline void Request_Color::set_g(::google::protobuf::int32 value) {
  set_has_g();
  g_ = value;
}

// required int32 b = 3;
inline bool Request_Color::has_b() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Request_Color::set_has_b() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Request_Color::clear_has_b() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Request_Color::clear_b() {
  b_ = 0;
  clear_has_b();
}
inline ::google::protobuf::int32 Request_Color::b() const {
  return b_;
}
inline void Request_Color::set_b(::google::protobuf::int32 value) {
  set_has_b();
  b_ = value;
}

// -------------------------------------------------------------------

// Request

// optional string imgPath = 1;
inline bool Request::has_imgpath() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Request::set_has_imgpath() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Request::clear_has_imgpath() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Request::clear_imgpath() {
  if (imgpath_ != &::google::protobuf::internal::kEmptyString) {
    imgpath_->clear();
  }
  clear_has_imgpath();
}
inline const ::std::string& Request::imgpath() const {
  return *imgpath_;
}
inline void Request::set_imgpath(const ::std::string& value) {
  set_has_imgpath();
  if (imgpath_ == &::google::protobuf::internal::kEmptyString) {
    imgpath_ = new ::std::string;
  }
  imgpath_->assign(value);
}
inline void Request::set_imgpath(const char* value) {
  set_has_imgpath();
  if (imgpath_ == &::google::protobuf::internal::kEmptyString) {
    imgpath_ = new ::std::string;
  }
  imgpath_->assign(value);
}
inline void Request::set_imgpath(const char* value, size_t size) {
  set_has_imgpath();
  if (imgpath_ == &::google::protobuf::internal::kEmptyString) {
    imgpath_ = new ::std::string;
  }
  imgpath_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Request::mutable_imgpath() {
  set_has_imgpath();
  if (imgpath_ == &::google::protobuf::internal::kEmptyString) {
    imgpath_ = new ::std::string;
  }
  return imgpath_;
}
inline ::std::string* Request::release_imgpath() {
  clear_has_imgpath();
  if (imgpath_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = imgpath_;
    imgpath_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Request::set_allocated_imgpath(::std::string* imgpath) {
  if (imgpath_ != &::google::protobuf::internal::kEmptyString) {
    delete imgpath_;
  }
  if (imgpath) {
    set_has_imgpath();
    imgpath_ = imgpath;
  } else {
    clear_has_imgpath();
    imgpath_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// repeated int32 id = 2;
inline int Request::id_size() const {
  return id_.size();
}
inline void Request::clear_id() {
  id_.Clear();
}
inline ::google::protobuf::int32 Request::id(int index) const {
  return id_.Get(index);
}
inline void Request::set_id(int index, ::google::protobuf::int32 value) {
  id_.Set(index, value);
}
inline void Request::add_id(::google::protobuf::int32 value) {
  id_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
Request::id() const {
  return id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
Request::mutable_id() {
  return &id_;
}

// repeated .dmir.Request.Color needColor = 3;
inline int Request::needcolor_size() const {
  return needcolor_.size();
}
inline void Request::clear_needcolor() {
  needcolor_.Clear();
}
inline const ::dmir::Request_Color& Request::needcolor(int index) const {
  return needcolor_.Get(index);
}
inline ::dmir::Request_Color* Request::mutable_needcolor(int index) {
  return needcolor_.Mutable(index);
}
inline ::dmir::Request_Color* Request::add_needcolor() {
  return needcolor_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >&
Request::needcolor() const {
  return needcolor_;
}
inline ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >*
Request::mutable_needcolor() {
  return &needcolor_;
}

// repeated .dmir.Request.Color denyColor = 4;
inline int Request::denycolor_size() const {
  return denycolor_.size();
}
inline void Request::clear_denycolor() {
  denycolor_.Clear();
}
inline const ::dmir::Request_Color& Request::denycolor(int index) const {
  return denycolor_.Get(index);
}
inline ::dmir::Request_Color* Request::mutable_denycolor(int index) {
  return denycolor_.Mutable(index);
}
inline ::dmir::Request_Color* Request::add_denycolor() {
  return denycolor_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >&
Request::denycolor() const {
  return denycolor_;
}
inline ::google::protobuf::RepeatedPtrField< ::dmir::Request_Color >*
Request::mutable_denycolor() {
  return &denycolor_;
}

// optional bytes image = 5;
inline bool Request::has_image() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Request::set_has_image() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Request::clear_has_image() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Request::clear_image() {
  if (image_ != &::google::protobuf::internal::kEmptyString) {
    image_->clear();
  }
  clear_has_image();
}
inline const ::std::string& Request::image() const {
  return *image_;
}
inline void Request::set_image(const ::std::string& value) {
  set_has_image();
  if (image_ == &::google::protobuf::internal::kEmptyString) {
    image_ = new ::std::string;
  }
  image_->assign(value);
}
inline void Request::set_image(const char* value) {
  set_has_image();
  if (image_ == &::google::protobuf::internal::kEmptyString) {
    image_ = new ::std::string;
  }
  image_->assign(value);
}
inline void Request::set_image(const void* value, size_t size) {
  set_has_image();
  if (image_ == &::google::protobuf::internal::kEmptyString) {
    image_ = new ::std::string;
  }
  image_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Request::mutable_image() {
  set_has_image();
  if (image_ == &::google::protobuf::internal::kEmptyString) {
    image_ = new ::std::string;
  }
  return image_;
}
inline ::std::string* Request::release_image() {
  clear_has_image();
  if (image_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = image_;
    image_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Request::set_allocated_image(::std::string* image) {
  if (image_ != &::google::protobuf::internal::kEmptyString) {
    delete image_;
  }
  if (image) {
    set_has_image();
    image_ = image;
  } else {
    clear_has_image();
    image_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// Result

// repeated string imgPath = 1;
inline int Result::imgpath_size() const {
  return imgpath_.size();
}
inline void Result::clear_imgpath() {
  imgpath_.Clear();
}
inline const ::std::string& Result::imgpath(int index) const {
  return imgpath_.Get(index);
}
inline ::std::string* Result::mutable_imgpath(int index) {
  return imgpath_.Mutable(index);
}
inline void Result::set_imgpath(int index, const ::std::string& value) {
  imgpath_.Mutable(index)->assign(value);
}
inline void Result::set_imgpath(int index, const char* value) {
  imgpath_.Mutable(index)->assign(value);
}
inline void Result::set_imgpath(int index, const char* value, size_t size) {
  imgpath_.Mutable(index)->assign(
    reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Result::add_imgpath() {
  return imgpath_.Add();
}
inline void Result::add_imgpath(const ::std::string& value) {
  imgpath_.Add()->assign(value);
}
inline void Result::add_imgpath(const char* value) {
  imgpath_.Add()->assign(value);
}
inline void Result::add_imgpath(const char* value, size_t size) {
  imgpath_.Add()->assign(reinterpret_cast<const char*>(value), size);
}
inline const ::google::protobuf::RepeatedPtrField< ::std::string>&
Result::imgpath() const {
  return imgpath_;
}
inline ::google::protobuf::RepeatedPtrField< ::std::string>*
Result::mutable_imgpath() {
  return &imgpath_;
}

// repeated int32 id = 2;
inline int Result::id_size() const {
  return id_.size();
}
inline void Result::clear_id() {
  id_.Clear();
}
inline ::google::protobuf::int32 Result::id(int index) const {
  return id_.Get(index);
}
inline void Result::set_id(int index, ::google::protobuf::int32 value) {
  id_.Set(index, value);
}
inline void Result::add_id(::google::protobuf::int32 value) {
  id_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
Result::id() const {
  return id_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
Result::mutable_id() {
  return &id_;
}

// repeated double precision = 3;
inline int Result::precision_size() const {
  return precision_.size();
}
inline void Result::clear_precision() {
  precision_.Clear();
}
inline double Result::precision(int index) const {
  return precision_.Get(index);
}
inline void Result::set_precision(int index, double value) {
  precision_.Set(index, value);
}
inline void Result::add_precision(double value) {
  precision_.Add(value);
}
inline const ::google::protobuf::RepeatedField< double >&
Result::precision() const {
  return precision_;
}
inline ::google::protobuf::RepeatedField< double >*
Result::mutable_precision() {
  return &precision_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace dmir

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_imgret_2eproto__INCLUDED
