// Do not edit. Bootstrap copy of /Volumes/Android/buildbot/src/android/build-tools/out/obj/go/src/cmd/internal/obj/textflag.go

//line /Volumes/Android/buildbot/src/android/build-tools/out/obj/go/src/cmd/internal/obj/textflag.go:1
// Copyright 2013 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// This file defines flags attached to various functions
// and data objects.  The compilers, assemblers, and linker must
// all agree on these values.

package obj

const (
	// Don't profile the marked routine.
	//
	// Deprecated: Not implemented, do not use.
	NOPROF = 1

	// It is ok for the linker to get multiple of these symbols.  It will
	// pick one of the duplicates to use.
	DUPOK = 2

	// Don't insert stack check preamble.
	NOSPLIT = 4

	// Put this data in a read-only section.
	RODATA = 8

	// This data contains no pointers.
	NOPTR = 16

	// This is a wrapper function and should not count as disabling 'recover'.
	WRAPPER = 32

	// This function uses its incoming context register.
	NEEDCTXT = 64

	// When passed to ggloblsym, causes Local to be set to true on the LSym it creates.
	LOCAL = 128
)
