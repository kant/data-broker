/*
 * Copyright © 2018 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "util/lock_tools.h"
#include "libdatabroker.h"
#include "libdatabroker_int.h"

#include <stddef.h>

DBR_Errorcode_t csPostProcessRequest( dbrRequestContext_t *rctx )
{
  if( rctx == NULL )
    return DBR_ERR_INVALID;

  DBR_Errorcode_t rc_out = DBR_ERR_GENERIC;
  switch( rctx->_req._opcode )
  {
    case DBBE_OPCODE_PUT:
    case DBBE_OPCODE_GET:
    case DBBE_OPCODE_READ:
      if( dbrCheck_response( rctx ) == DBR_SUCCESS )
        rc_out = DBR_SUCCESS;
      break;

    default:
      break;
  }

  return rc_out;
}

DBR_Errorcode_t
libdbrTest( DBR_Tag_t req_tag)
{
#ifdef DBR_INTTAG

  if(( req_tag < 0 ) || ( req_tag >= dbrMAX_TAGS ))
    return DBR_ERR_TAGERROR;

  dbrMain_context_t *main_ctx = dbrCheckCreateMainCTX();
  if( main_ctx == NULL )
    return DBR_ERR_INVALID;

  BIGLOCK_LOCK( main_ctx );

  dbrRequestContext_t *rctx = main_ctx->_cs_wq[ req_tag ];

#else
#error "Currently not supported because of lack of access to main context and locking"
  if( req_tag == NULL
      || req_tag == DB_TAG_ERROR )
    return DBR_ERR_TAGERROR;

  dbrRequestContext_t *rctx = *(dbrRequestContext_t**)req_tag;
  if( rctx == NULL )
    return DBR_ERR_TAGERROR;

#endif


  DBR_Errorcode_t rc = dbrValidateTag( rctx, req_tag );
  if( rc != DBR_SUCCESS )
    BIGLOCK_UNLOCKRETURN( main_ctx, rc );

  dbrName_space_t* cs = rctx->_ctx;
  if( cs->_be_ctx == NULL )
    BIGLOCK_UNLOCKRETURN( main_ctx, DBR_ERR_NSINVAL );

  rc = dbrTest_request( cs, rctx );
  switch( rc )
  {
    case DBR_SUCCESS:
      // success, now see if there were any chained requests
      rc = csPostProcessRequest( rctx );
      break;

    case DBR_ERR_INPROGRESS:
      // request still pending, so don't touch the book-keeping,
      // but let outside know that it is pending!

      // todo: for now keeping this case separate despite being empty
      BIGLOCK_UNLOCKRETURN( main_ctx, DBR_ERR_INPROGRESS );

    default:
      break;
  }
  dbrRemove_request( rctx->_ctx, rctx );

  BIGLOCK_UNLOCKRETURN( main_ctx, rc );
}
