/*
 * Record values functions
 *
 * Copyright (c) 2011-2012, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libevtx_io_handle.h"
#include "libevtx_libcerror.h"
#include "libevtx_libcnotify.h"
#include "libevtx_libfdatetime.h"
#include "libevtx_libfvalue.h"
#include "libevtx_libfwevt.h"
#include "libevtx_record_values.h"

#include "evtx_event_record.h"

const uint8_t evtx_event_record_signature[ 4 ] = { 0x2a, 0x2a, 0x00, 0x00 };

/* Initialize record values
 * Make sure the value record values is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_initialize(
     libevtx_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_initialize";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( *record_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values value already set.",
		 function );

		return( -1 );
	}
	*record_values = memory_allocate_structure(
	                  libevtx_record_values_t );

	if( *record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record values.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *record_values,
	     0,
	     sizeof( libevtx_record_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *record_values != NULL )
	{
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( -1 );
}

/* Frees record values
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_free(
     libevtx_record_values_t **record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_free";
	int result            = 1;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( *record_values != NULL )
	{
		if( ( *record_values )->xml_document != NULL )
		{
			if( libfwevt_xml_document_free(
			     &( ( *record_values )->xml_document ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free XML document.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *record_values );

		*record_values = NULL;
	}
	return( result );
}

/* Clones the record values
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_clone(
     libevtx_record_values_t **destination_record_values,
     libevtx_record_values_t *source_record_values,
     libcerror_error_t **error )
{
	static char *function = "libevtx_record_values_free";

	if( destination_record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination record values.",
		 function );

		return( -1 );
	}
	if( *destination_record_values != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination record values value already set.",
		 function );

		return( -1 );
	}
	if( source_record_values == NULL )
	{
		*destination_record_values = NULL;

		return( 1 );
	}
	*destination_record_values = memory_allocate_structure(
	                              libevtx_record_values_t );

	if( *destination_record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create destination record values.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *destination_record_values,
	     source_record_values,
	     sizeof( libevtx_record_values_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy record values.",
		 function );

		goto on_error;
	}
	( *destination_record_values )->xml_document = NULL;

	if( source_record_values->xml_document != NULL )
	{
		if( libfwevt_xml_document_clone(
		     &( ( *destination_record_values )->xml_document ),
		     source_record_values->xml_document,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create destination XML document.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *destination_record_values != NULL )
	{
		memory_free(
		 *destination_record_values );

		*destination_record_values = NULL;
	}
	return( -1 );
}

/* Reads the record values header
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_read_header(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     size_t chunk_data_offset,
     libcerror_error_t **error )
{
	const uint8_t *event_record_data  = NULL;
	static char *function             = "libevtx_record_values_read_header";
	size_t event_record_data_size     = 0;
	uint32_t copy_of_size             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t filetime_string[ 32 ];

	libfdatetime_filetime_t *filetime = NULL;
	int result                        = 0;
#endif

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	event_record_data      = &( chunk_data[ chunk_data_offset ] );
	event_record_data_size = chunk_data_size - chunk_data_offset;

	if( event_record_data_size < ( sizeof( evtx_event_record_header_t ) + 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid event record data size value too small.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: event record header data:\n",
		 function );
		libcnotify_print_data(
		 event_record_data,
		 sizeof( evtx_event_record_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     ( (evtx_event_record_header_t *) event_record_data )->signature,
	     evtx_event_record_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported event record signature.",
		 function );

		goto on_error;
	}
	record_values->chunk_data_offset = chunk_data_offset;

	byte_stream_copy_to_uint32_little_endian(
	 ( (evtx_event_record_header_t *) event_record_data )->size,
	 record_values->data_size );

	byte_stream_copy_to_uint64_little_endian(
	 ( (evtx_event_record_header_t *) event_record_data )->identifier,
	 record_values->identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (evtx_event_record_header_t *) event_record_data )->written_time,
	 record_values->written_time );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: \\x%02x\\x%02x\\x%02x\\x%02x\n",
		 function,
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 0 ],
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 1 ],
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 2 ] ,
		 ( (evtx_event_record_header_t *) event_record_data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 record_values->data_size );

		libcnotify_printf(
		 "%s: identifier\t\t\t\t: %" PRIu64 "\n",
		 function,
		 record_values->identifier );

		if( libfdatetime_filetime_initialize(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_filetime_copy_from_byte_stream(
		     filetime,
		     ( (evtx_event_record_header_t *) event_record_data )->written_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy file time from byte stream.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_filetime_copy_to_utf16_string(
			  filetime,
			  (uint16_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_filetime_copy_to_utf8_string(
			  filetime,
			  (uint8_t *) filetime_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy file time to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: written time\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM " UTC\n",
		 function,
		 filetime_string );

		if( libfdatetime_filetime_free(
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file time.",
			 function );

			goto on_error;
		}
	}
#endif
	if( ( record_values->data_size < sizeof( evtx_event_record_header_t ) )
	 || ( record_values->data_size > ( event_record_data_size - 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid event record data size value out of bounds.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( event_record_data[ record_values->data_size - 4 ] ),
	 copy_of_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: copy of size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 copy_of_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO validate size with copy */
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( filetime != NULL )
	{
		libfdatetime_filetime_free(
		 &filetime,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads the record values XML document
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_read_xml_document(
     libevtx_record_values_t *record_values,
     libevtx_io_handle_t *io_handle,
     const uint8_t *chunk_data,
     size_t chunk_data_size,
     libcerror_error_t **error )
{
	const uint8_t *event_record_data  = NULL;
	static char *function             = "libevtx_record_values_read_xml_document";
	size_t chunk_data_offset          = 0;
	size_t event_record_data_size     = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record values - XML document already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( chunk_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid chunk data.",
		 function );

		return( -1 );
	}
	if( chunk_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid chunk data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( record_values->chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record values - chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	chunk_data_offset      = record_values->chunk_data_offset
	                       + sizeof( evtx_event_record_header_t );
	event_record_data_size = record_values->data_size
	                       - ( sizeof( evtx_event_record_header_t ) + 4 );

	if( chunk_data_offset >= chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid chunk data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( chunk_data_offset + event_record_data_size ) > chunk_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid event record data size value out of bounds.",
		 function );

		goto on_error;
	}
	event_record_data = &( chunk_data[ chunk_data_offset ] );

	if( libfwevt_xml_document_initialize(
	     &( record_values->xml_document ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create XML document.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: event record data:\n",
		 function );
		libcnotify_print_data(
		 event_record_data,
		 event_record_data_size,
		 0 );
	}
#endif
	if( libfwevt_xml_document_read(
	     record_values->xml_document,
	     chunk_data,
	     chunk_data_size,
	     chunk_data_offset,
	     io_handle->ascii_codepage,
	     LIBFWEVT_XML_DOCUMENT_READ_FLAG_HAS_DATA_OFFSETS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read binary XML document.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: XML document:\n",
		 function );

		if( libfwevt_xml_document_debug_print(
		     record_values->xml_document,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print XML document.",
			 function );

			goto on_error;
		}
	}
#endif
	return( 1 );

on_error:
	if( record_values->xml_document != NULL )
	{
		libfwevt_xml_document_free(
		 &( record_values->xml_document ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the event identifier
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_event_identifier(
     libevtx_record_values_t *record_values,
     uint32_t *event_identifier,
     libcerror_error_t **error )
{
	libfvalue_value_t *qualifiers_value    = NULL;
	libfwevt_xml_tag_t *eventid_xml_tag    = NULL;
	libfwevt_xml_tag_t *qualifiers_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag       = NULL;
	libfwevt_xml_tag_t *system_xml_tag     = NULL;
	static char *function                  = "libevtx_record_values_get_event_identifier";
	uint32_t qualifiers                    = 0;
	int result                             = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_identifier_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     root_xml_tag,
		     (uint8_t *) "System",
		     6,
		     &system_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     system_xml_tag,
		     (uint8_t *) "EventID",
		     7,
		     &eventid_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventID XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_value(
		     eventid_xml_tag,
		     &( record_values->event_identifier_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventID XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_32bit(
	     record_values->event_identifier_value,
	     0,
	     event_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value to event identifier.",
		 function );

		return( -1 );
	}
	result = libfwevt_xml_tag_get_attribute_by_utf8_name(
	          eventid_xml_tag,
	          (uint8_t *) "Qualifiers",
	          10,
	          &qualifiers_xml_tag,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve Qualifiers XML attribute.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfwevt_xml_tag_get_value(
		     qualifiers_xml_tag,
		     &qualifiers_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Qualifiers XML element value.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_copy_to_32bit(
		     qualifiers_value,
		     0,
		     &qualifiers,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to qualifiers.",
			 function );

			return( -1 );
		}
		qualifiers <<= 16;

		*event_identifier |= qualifiers;
	}
	return( 1 );
}

/* Retrieves the event level
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_event_level(
     libevtx_record_values_t *record_values,
     uint8_t *event_level,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *level_xml_tag  = NULL;
	libfwevt_xml_tag_t *root_xml_tag   = NULL;
	libfwevt_xml_tag_t *system_xml_tag = NULL;
	static char *function              = "libevtx_record_values_get_event_level";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->level_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     root_xml_tag,
		     (uint8_t *) "System",
		     6,
		     &system_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_element_by_utf8_name(
		     system_xml_tag,
		     (uint8_t *) "Level",
		     5,
		     &level_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Level XML element.",
			 function );

			return( -1 );
		}
		if( libfwevt_xml_tag_get_value(
		     level_xml_tag,
		     &( record_values->level_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Level XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_8bit(
	     record_values->level_value,
	     0,
	     event_level,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value to event level.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_source_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_xml_tag      = NULL;
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf8_source_name_size";
	int result                                = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_name_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &provider_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf8_string_size(
	     record_values->provider_name_value,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of source name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_source_name(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_xml_tag      = NULL;
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf8_source_name";
	int result                                = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_name_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &provider_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf8_string(
	     record_values->provider_name_value,
	     0,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy source name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded source name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_source_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_xml_tag      = NULL;
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf16_source_name_size";
	int result                                = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_name_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &provider_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf16_string_size(
	     record_values->provider_name_value,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of source name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded source name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_source_name(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *provider_xml_tag      = NULL;
	libfwevt_xml_tag_t *provider_name_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag          = NULL;
	libfwevt_xml_tag_t *system_xml_tag        = NULL;
	static char *function                     = "libevtx_record_values_get_utf16_source_name";
	int result                                = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->provider_name_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Provider",
		          8,
		          &provider_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Provider XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_attribute_by_utf8_name(
		          provider_xml_tag,
		          (uint8_t *) "EventSourceName",
		          15,
		          &provider_name_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventSourceName XML attribute.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			result = libfwevt_xml_tag_get_attribute_by_utf8_name(
				  provider_xml_tag,
				  (uint8_t *) "Name",
				  4,
				  &provider_name_xml_tag,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve Name XML attribute.",
				 function );

				return( -1 );
			}
		}
		if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     provider_name_xml_tag,
		     &( record_values->provider_name_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve provider name XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf16_string(
	     record_values->provider_name_value,
	     0,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy source name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_computer_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf8_computer_name_size";
	int result                           = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf8_string_size(
	     record_values->computer_value,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of computer name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf8_computer_name(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf8_computer_name";
	int result                           = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf8_string(
	     record_values->computer_value,
	     0,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy computer name to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded computer name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_computer_name_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf16_computer_name_size";
	int result                           = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_utf16_string_size(
	     record_values->computer_value,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of computer name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded computer name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_utf16_computer_name(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *computer_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag     = NULL;
	libfwevt_xml_tag_t *system_xml_tag   = NULL;
	static char *function                = "libevtx_record_values_get_utf16_computer_name";
	int result                           = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->computer_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          root_xml_tag,
		          (uint8_t *) "System",
		          6,
		          &system_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve System XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
		          system_xml_tag,
		          (uint8_t *) "Computer",
		          8,
		          &computer_xml_tag,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve Computer XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     computer_xml_tag,
		     &( record_values->computer_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve computer XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_to_utf16_string(
	     record_values->computer_value,
	     0,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy computer name to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the event data XML tag
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_event_data_xml_tag(
     libevtx_record_values_t *record_values,
     libfwevt_xml_tag_t **event_data_xml_tag,
     uint8_t *event_data_type,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *user_data_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag      = NULL;
	static char *function                 = "libevtx_record_values_get_event_data_xml_tag";
	int number_of_elements                = 0;
	int result                            = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( event_data_xml_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event data XML tag.",
		 function );

		return( -1 );
	}
	if( event_data_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid event data type.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_document_get_root_xml_tag(
	     record_values->xml_document,
	     &root_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML element.",
		 function );

		return( -1 );
	}
	/* The event data is either stored in the EventData XML tag
	 */
	result = libfwevt_xml_tag_get_element_by_utf8_name(
		  root_xml_tag,
		  (uint8_t *) "EventData",
		  9,
		  event_data_xml_tag,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve EventData XML element.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		*event_data_type = LIBEVTX_RECORD_VALUES_EVENT_DATA_TYPE_EVENT_DATA;
	}
	else
	{
		/* Or the event data is either stored in the UserData XML tag
		 */
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "UserData",
			  8,
			  &user_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UserData XML element.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( libfwevt_xml_tag_get_number_of_elements(
			     user_data_xml_tag,
			     &number_of_elements,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of elements.",
				 function );

				return( -1 );
			}
			if( number_of_elements != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported number of UserData elements.",
				 function );

				return( -1 );
			}
			if( libfwevt_xml_tag_get_element_by_index(
			     user_data_xml_tag,
			     0,
			     event_data_xml_tag,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve first UserData element.",
				 function );

				return( -1 );
			}
			*event_data_type = LIBEVTX_RECORD_VALUES_EVENT_DATA_TYPE_USER_DATA;
		}
	}
	return( result );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_number_of_strings(
     libevtx_record_values_t *record_values,
     int *number_of_strings,
     libcerror_error_t **error )
{
	uint8_t element_name[ 5 ];

	libfwevt_xml_tag_t *element_xml_tag   = NULL;
	libfwevt_xml_tag_t *user_data_xml_tag = NULL;
	static char *function                 = "libevtx_record_values_get_number_of_strings";
	size_t element_name_size              = 0;
	int element_index                     = 0;
	int number_of_elements                = 0;
	int result                            = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( number_of_strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of strings.",
		 function );

		return( -1 );
	}
	*number_of_strings = 0;

	if( record_values->event_data_xml_tag == NULL )
	{
		result = libevtx_record_values_get_event_data_xml_tag(
			  record_values,
			  &( record_values->event_data_xml_tag ),
			  &( record_values->event_data_type ),
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event data XML tag.",
			 function );

			return( -1 );
		}
	}
	if( record_values->event_data_xml_tag != NULL )
	{
		if( libfwevt_xml_tag_get_number_of_elements(
		     record_values->event_data_xml_tag,
		     &number_of_elements,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of elements.",
			 function );

			return( -1 );
		}
		if( record_values->event_data_type == LIBEVTX_RECORD_VALUES_EVENT_DATA_TYPE_USER_DATA )
		{
			*number_of_strings = number_of_elements;
		}
		else
		{
			for( element_index = 0;
			     element_index < number_of_elements;
			     element_index++ )
			{
				if( libfwevt_xml_tag_get_element_by_index(
				     record_values->event_data_xml_tag,
				     element_index,
				     &element_xml_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve element: %d.",
					 function,
					 element_index );

					return( -1 );
				}
				if( element_xml_tag == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: missing element: %d.",
					 function,
					 element_index );

					return( -1 );
				}
				if( libfwevt_xml_tag_get_utf8_name_size(
				     element_xml_tag,
				     &element_name_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve element: %d name size.",
					 function,
					 element_index );

					return( -1 );
				}
				if( element_name_size == 5 )
				{
					if( libfwevt_xml_tag_get_utf8_name(
					     element_xml_tag,
					     element_name,
					     5,
					     error ) != 1 )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_RUNTIME,
						 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
						 "%s: unable to retrieve element: %d name.",
						 function,
						 element_index );

						return( -1 );
					}
					if( libcstring_narrow_string_compare(
					     (char *) element_name,
					     "Data",
					     4 ) == 0 )
					{
						if( element_index != *number_of_strings )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
							 "%s: unsupported non-contiguous Data elements.",
							 function );

							return( -1 );
						}
						*number_of_strings += 1;
					}
				}
			}
		}
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf8_string_size(
     libevtx_record_values_t *record_values,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *element_xml_tag = NULL;
	static char *function               = "libevtx_record_values_get_utf8_string_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_data_xml_tag == NULL )
	{
		if( libevtx_record_values_get_event_data_xml_tag(
		     record_values,
		     &( record_values->event_data_xml_tag ),
		     &( record_values->event_data_type ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event data XML tag.",
			 function );

			return( -1 );
		}
	}
	if( libfwevt_xml_tag_get_element_by_index(
	     record_values->event_data_xml_tag,
	     string_index,
	     &element_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf8_value_size(
	     element_xml_tag,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d value size.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf8_string(
     libevtx_record_values_t *record_values,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *element_xml_tag = NULL;
	static char *function               = "libevtx_record_values_get_utf8_string";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_data_xml_tag == NULL )
	{
		if( libevtx_record_values_get_event_data_xml_tag(
		     record_values,
		     &( record_values->event_data_xml_tag ),
		     &( record_values->event_data_type ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event data XML tag.",
			 function );

			return( -1 );
		}
	}
	if( libfwevt_xml_tag_get_element_by_index(
	     record_values->event_data_xml_tag,
	     string_index,
	     &element_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf8_value(
	     element_xml_tag,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d value.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of a specific UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf16_string_size(
     libevtx_record_values_t *record_values,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *element_xml_tag = NULL;
	static char *function               = "libevtx_record_values_get_utf16_string_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_data_xml_tag == NULL )
	{
		if( libevtx_record_values_get_event_data_xml_tag(
		     record_values,
		     &( record_values->event_data_xml_tag ),
		     &( record_values->event_data_type ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event data XML tag.",
			 function );

			return( -1 );
		}
	}
	if( libfwevt_xml_tag_get_element_by_index(
	     record_values->event_data_xml_tag,
	     string_index,
	     &element_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf16_value_size(
	     element_xml_tag,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d value size.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf16_string(
     libevtx_record_values_t *record_values,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *element_xml_tag = NULL;
	static char *function               = "libevtx_record_values_get_utf16_string";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->event_data_xml_tag == NULL )
	{
		if( libevtx_record_values_get_event_data_xml_tag(
		     record_values,
		     &( record_values->event_data_xml_tag ),
		     &( record_values->event_data_type ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve event data XML tag.",
			 function );

			return( -1 );
		}
	}
	if( libfwevt_xml_tag_get_element_by_index(
	     record_values->event_data_xml_tag,
	     string_index,
	     &element_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d.",
		 function,
		 string_index );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf16_value(
	     element_xml_tag,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve element: %d value.",
		 function,
		 string_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_data_size(
     libevtx_record_values_t *record_values,
     size_t *data_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *binary_data_tag    = NULL;
	libfwevt_xml_tag_t *element_xml_tag    = NULL;
	libfwevt_xml_tag_t *event_data_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag       = NULL;
	static char *function                  = "libevtx_record_values_get_data_size";
	int result                             = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->binary_data_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "EventData",
			  9,
			  &event_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  event_data_xml_tag,
			  (uint8_t *) "BinaryData",
			  10,
			  &binary_data_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     binary_data_tag,
		     &( record_values->binary_data_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_get_data_size(
	     record_values->binary_data_value,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of binary data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libevtx_record_values_get_data(
     libevtx_record_values_t *record_values,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *binary_data_tag    = NULL;
	libfwevt_xml_tag_t *element_xml_tag    = NULL;
	libfwevt_xml_tag_t *event_data_xml_tag = NULL;
	libfwevt_xml_tag_t *root_xml_tag       = NULL;
	static char *function                  = "libevtx_record_values_get_data";
	int result                             = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( record_values->binary_data_value == NULL )
	{
		if( libfwevt_xml_document_get_root_xml_tag(
		     record_values->xml_document,
		     &root_xml_tag,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root XML element.",
			 function );

			return( -1 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  root_xml_tag,
			  (uint8_t *) "EventData",
			  9,
			  &event_data_xml_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve EventData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		result = libfwevt_xml_tag_get_element_by_utf8_name(
			  event_data_xml_tag,
			  (uint8_t *) "BinaryData",
			  10,
			  &binary_data_tag,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			return( 0 );
		}
		if( libfwevt_xml_tag_get_value(
		     binary_data_tag,
		     &( record_values->binary_data_value ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve BinaryData XML element value.",
			 function );

			return( -1 );
		}
	}
	if( libfvalue_value_copy_data(
	     record_values->binary_data_value,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy binary data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf8_xml_string_size(
     libevtx_record_values_t *record_values,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag = NULL;
	static char *function            = "libevtx_record_values_get_utf8_xml_string_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_document_get_root_xml_tag(
	     record_values->xml_document,
	     &root_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML element.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf8_xml_string_size(
	     root_xml_tag,
	     0,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size of XML document root tag.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf8_xml_string(
     libevtx_record_values_t *record_values,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag = NULL;
	static char *function            = "libevtx_record_values_get_utf8_xml_string";
	size_t utf8_string_index         = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_document_get_root_xml_tag(
	     record_values->xml_document,
	     &root_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML element.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf8_xml_string_with_index(
	     root_xml_tag,
	     0,
	     utf8_string,
	     utf8_string_size,
	     &utf8_string_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy XML document root tag to UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded XML string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf16_xml_string_size(
     libevtx_record_values_t *record_values,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag = NULL;
	static char *function            = "libevtx_record_values_get_utf16_xml_string_size";

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_document_get_root_xml_tag(
	     record_values->xml_document,
	     &root_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML element.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf16_xml_string_size(
	     root_xml_tag,
	     0,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size of XML document root tag.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded XML string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libevtx_record_values_get_utf16_xml_string(
     libevtx_record_values_t *record_values,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfwevt_xml_tag_t *root_xml_tag = NULL;
	static char *function            = "libevtx_record_values_get_utf16_xml_string";
	size_t utf16_string_index        = 0;

	if( record_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record values.",
		 function );

		return( -1 );
	}
	if( record_values->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record values - missing XML document.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_document_get_root_xml_tag(
	     record_values->xml_document,
	     &root_xml_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML element.",
		 function );

		return( -1 );
	}
	if( libfwevt_xml_tag_get_utf16_xml_string_with_index(
	     root_xml_tag,
	     0,
	     utf16_string,
	     utf16_string_size,
	     &utf16_string_index,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy XML document root tag to UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

