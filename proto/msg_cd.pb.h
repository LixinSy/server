// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: msg_cd.proto

#ifndef PROTOBUF_msg_5fcd_2eproto__INCLUDED
#define PROTOBUF_msg_5fcd_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
#include "msg_def.pb.h"
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_msg_5fcd_2eproto();
void protobuf_AssignDesc_msg_5fcd_2eproto();
void protobuf_ShutdownFile_msg_5fcd_2eproto();

class KeepLiveRequest;
class KeepLiveResponse;

// ===================================================================

class KeepLiveRequest : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:KeepLiveRequest) */ {
 public:
  KeepLiveRequest();
  virtual ~KeepLiveRequest();

  KeepLiveRequest(const KeepLiveRequest& from);

  inline KeepLiveRequest& operator=(const KeepLiveRequest& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const KeepLiveRequest& default_instance();

  void Swap(KeepLiveRequest* other);

  // implements Message ----------------------------------------------

  inline KeepLiveRequest* New() const { return New(NULL); }

  KeepLiveRequest* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const KeepLiveRequest& from);
  void MergeFrom(const KeepLiveRequest& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(KeepLiveRequest* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .ReqHead head = 1;
  bool has_head() const;
  void clear_head();
  static const int kHeadFieldNumber = 1;
  const ::ReqHead& head() const;
  ::ReqHead* mutable_head();
  ::ReqHead* release_head();
  void set_allocated_head(::ReqHead* head);

  // @@protoc_insertion_point(class_scope:KeepLiveRequest)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::ReqHead* head_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_msg_5fcd_2eproto();
  friend void protobuf_AssignDesc_msg_5fcd_2eproto();
  friend void protobuf_ShutdownFile_msg_5fcd_2eproto();

  void InitAsDefaultInstance();
  static KeepLiveRequest* default_instance_;
};
// -------------------------------------------------------------------

class KeepLiveResponse : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:KeepLiveResponse) */ {
 public:
  KeepLiveResponse();
  virtual ~KeepLiveResponse();

  KeepLiveResponse(const KeepLiveResponse& from);

  inline KeepLiveResponse& operator=(const KeepLiveResponse& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const KeepLiveResponse& default_instance();

  void Swap(KeepLiveResponse* other);

  // implements Message ----------------------------------------------

  inline KeepLiveResponse* New() const { return New(NULL); }

  KeepLiveResponse* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const KeepLiveResponse& from);
  void MergeFrom(const KeepLiveResponse& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const {
    return InternalSerializeWithCachedSizesToArray(false, output);
  }
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(KeepLiveResponse* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .RespHead head = 1;
  bool has_head() const;
  void clear_head();
  static const int kHeadFieldNumber = 1;
  const ::RespHead& head() const;
  ::RespHead* mutable_head();
  ::RespHead* release_head();
  void set_allocated_head(::RespHead* head);

  // optional uint32 timestump = 2;
  void clear_timestump();
  static const int kTimestumpFieldNumber = 2;
  ::google::protobuf::uint32 timestump() const;
  void set_timestump(::google::protobuf::uint32 value);

  // @@protoc_insertion_point(class_scope:KeepLiveResponse)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::RespHead* head_;
  ::google::protobuf::uint32 timestump_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_msg_5fcd_2eproto();
  friend void protobuf_AssignDesc_msg_5fcd_2eproto();
  friend void protobuf_ShutdownFile_msg_5fcd_2eproto();

  void InitAsDefaultInstance();
  static KeepLiveResponse* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// KeepLiveRequest

// optional .ReqHead head = 1;
inline bool KeepLiveRequest::has_head() const {
  return !_is_default_instance_ && head_ != NULL;
}
inline void KeepLiveRequest::clear_head() {
  if (GetArenaNoVirtual() == NULL && head_ != NULL) delete head_;
  head_ = NULL;
}
inline const ::ReqHead& KeepLiveRequest::head() const {
  // @@protoc_insertion_point(field_get:KeepLiveRequest.head)
  return head_ != NULL ? *head_ : *default_instance_->head_;
}
inline ::ReqHead* KeepLiveRequest::mutable_head() {
  
  if (head_ == NULL) {
    head_ = new ::ReqHead;
  }
  // @@protoc_insertion_point(field_mutable:KeepLiveRequest.head)
  return head_;
}
inline ::ReqHead* KeepLiveRequest::release_head() {
  // @@protoc_insertion_point(field_release:KeepLiveRequest.head)
  
  ::ReqHead* temp = head_;
  head_ = NULL;
  return temp;
}
inline void KeepLiveRequest::set_allocated_head(::ReqHead* head) {
  delete head_;
  head_ = head;
  if (head) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:KeepLiveRequest.head)
}

// -------------------------------------------------------------------

// KeepLiveResponse

// optional .RespHead head = 1;
inline bool KeepLiveResponse::has_head() const {
  return !_is_default_instance_ && head_ != NULL;
}
inline void KeepLiveResponse::clear_head() {
  if (GetArenaNoVirtual() == NULL && head_ != NULL) delete head_;
  head_ = NULL;
}
inline const ::RespHead& KeepLiveResponse::head() const {
  // @@protoc_insertion_point(field_get:KeepLiveResponse.head)
  return head_ != NULL ? *head_ : *default_instance_->head_;
}
inline ::RespHead* KeepLiveResponse::mutable_head() {
  
  if (head_ == NULL) {
    head_ = new ::RespHead;
  }
  // @@protoc_insertion_point(field_mutable:KeepLiveResponse.head)
  return head_;
}
inline ::RespHead* KeepLiveResponse::release_head() {
  // @@protoc_insertion_point(field_release:KeepLiveResponse.head)
  
  ::RespHead* temp = head_;
  head_ = NULL;
  return temp;
}
inline void KeepLiveResponse::set_allocated_head(::RespHead* head) {
  delete head_;
  head_ = head;
  if (head) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:KeepLiveResponse.head)
}

// optional uint32 timestump = 2;
inline void KeepLiveResponse::clear_timestump() {
  timestump_ = 0u;
}
inline ::google::protobuf::uint32 KeepLiveResponse::timestump() const {
  // @@protoc_insertion_point(field_get:KeepLiveResponse.timestump)
  return timestump_;
}
inline void KeepLiveResponse::set_timestump(::google::protobuf::uint32 value) {
  
  timestump_ = value;
  // @@protoc_insertion_point(field_set:KeepLiveResponse.timestump)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_msg_5fcd_2eproto__INCLUDED
