--- ./source3/winbindd/winbindd_cred_cache.c.orig	2015-01-28 00:19:27.870438534 -0800
+++ ./source3/winbindd/winbindd_cred_cache.c	2015-01-28 00:22:09.013421232 -0800
@@ -802,62 +802,7 @@
 static NTSTATUS store_memory_creds(struct WINBINDD_MEMORY_CREDS *memcredp,
 				   const char *pass)
 {
-#if !defined(HAVE_MLOCK)
 	return NT_STATUS_OK;
-#else
-	/* new_entry->nt_hash is the base pointer for the block
-	   of memory pointed into by new_entry->lm_hash and
-	   new_entry->pass (if we're storing plaintext). */
-
-	memcredp->len = NT_HASH_LEN + LM_HASH_LEN;
-	if (pass) {
-		memcredp->len += strlen(pass)+1;
-	}
-
-
-#if defined(LINUX)
-	/* aligning the memory on on x86_64 and compiling
-	   with gcc 4.1 using -O2 causes a segv in the
-	   next memset()  --jerry */
-	memcredp->nt_hash = SMB_MALLOC_ARRAY(unsigned char, memcredp->len);
-#else
-	/* On non-linux platforms, mlock()'d memory must be aligned */
-	memcredp->nt_hash = SMB_MEMALIGN_ARRAY(unsigned char,
-					       getpagesize(), memcredp->len);
-#endif
-	if (!memcredp->nt_hash) {
-		return NT_STATUS_NO_MEMORY;
-	}
-	memset(memcredp->nt_hash, 0x0, memcredp->len);
-
-	memcredp->lm_hash = memcredp->nt_hash + NT_HASH_LEN;
-
-#ifdef DEBUG_PASSWORD
-	DEBUG(10,("mlocking memory: %p\n", memcredp->nt_hash));
-#endif
-	if ((mlock(memcredp->nt_hash, memcredp->len)) == -1) {
-		DEBUG(0,("failed to mlock memory: %s (%d)\n",
-			strerror(errno), errno));
-		SAFE_FREE(memcredp->nt_hash);
-		return map_nt_error_from_unix(errno);
-	}
-
-#ifdef DEBUG_PASSWORD
-	DEBUG(10,("mlocked memory: %p\n", memcredp->nt_hash));
-#endif
-
-	if (pass) {
-		/* Create and store the password hashes. */
-		E_md4hash(pass, memcredp->nt_hash);
-		E_deshash(pass, memcredp->lm_hash);
-
-		memcredp->pass = (char *)memcredp->lm_hash + LM_HASH_LEN;
-		memcpy(memcredp->pass, pass,
-		       memcredp->len - NT_HASH_LEN - LM_HASH_LEN);
-	}
-
-	return NT_STATUS_OK;
-#endif
 }
 
 /***********************************************************
@@ -907,61 +852,7 @@
 						   const char *pass)
 {
 	/* Shortcut to ensure we don't store if no mlock. */
-#if !defined(HAVE_MLOCK) || !defined(HAVE_MUNLOCK)
 	return NT_STATUS_OK;
-#else
-	NTSTATUS status;
-	struct WINBINDD_MEMORY_CREDS *memcredp = NULL;
-
-	memcredp = find_memory_creds_by_name(username);
-	if (uid == (uid_t)-1) {
-		DEBUG(0,("winbindd_add_memory_creds_internal: "
-			"invalid uid for user %s.\n", username));
-		return NT_STATUS_INVALID_PARAMETER;
-	}
-
-	if (memcredp) {
-		/* Already exists. Increment the reference count and replace stored creds. */
-		if (uid != memcredp->uid) {
-			DEBUG(0,("winbindd_add_memory_creds_internal: "
-				"uid %u for user %s doesn't "
-				"match stored uid %u. Replacing.\n",
-				(unsigned int)uid, username,
-				(unsigned int)memcredp->uid));
-			memcredp->uid = uid;
-		}
-		memcredp->ref_count++;
-		DEBUG(10,("winbindd_add_memory_creds_internal: "
-			"ref count for user %s is now %d\n",
-			username, memcredp->ref_count));
-		return winbindd_replace_memory_creds_internal(memcredp, pass);
-	}
-
-	memcredp = talloc_zero(NULL, struct WINBINDD_MEMORY_CREDS);
-	if (!memcredp) {
-		return NT_STATUS_NO_MEMORY;
-	}
-	memcredp->username = talloc_strdup(memcredp, username);
-	if (!memcredp->username) {
-		talloc_destroy(memcredp);
-		return NT_STATUS_NO_MEMORY;
-	}
-
-	status = store_memory_creds(memcredp, pass);
-	if (!NT_STATUS_IS_OK(status)) {
-		talloc_destroy(memcredp);
-		return status;
-	}
-
-	memcredp->uid = uid;
-	memcredp->ref_count = 1;
-	DLIST_ADD(memory_creds_list, memcredp);
-
-	DEBUG(10,("winbindd_add_memory_creds_internal: "
-		"added entry for user %s\n", username));
-
-	return NT_STATUS_OK;
-#endif
 }
 
 /*************************************************************
