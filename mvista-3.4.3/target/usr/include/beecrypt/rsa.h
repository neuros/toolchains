/*
 * Copyright (c) 2000, 2002 Virtual Unlimited B.V.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/*!\file rsa.h
 * \brief RSA algorithm.
 * \author Bob Deblier <bob.deblier@pandora.be>
 * \ingroup IF_m IF_rsa_m
 */

#ifndef _RSA_H
#define _RSA_H

#include "rsakp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!\fn int rsapub(const rsapk* pk, const mpnumber* m, mpnumber* c)
 * \brief This function performs a raw RSA public key operation.
 *
 * This function can be used for encryption and verifying.
 *
 * It performs the following operation:
 * \li \f$c=m^{e}\ \textrm{mod}\ n\f$
 *
 * \param pk The RSA public key.
 * \param m The message.
 * \param c The ciphertext.
 * \retval 0 on success.
 * \retval -1 on failure.
 */
BEECRYPTAPI
int rsapub   (const rsapk* pk, const mpnumber* m, mpnumber* c);

/*!\fn int rsapri(const rsakp* kp, const mpnumber* c, mpnumber* m)
 * \brief This function performs a raw RSA private key operation.
 *
 * This function can be used for decryption and signing.
 *
 * It performs the operation:
 * \li \f$m=c^{d}\ \textrm{mod}\ n\f$
 *
 * \param kp The RSA keypair.
 * \param c The ciphertext.
 * \param m The message.
 * \retval 0 on success.
 * \retval -1 on failure.
 */
BEECRYPTAPI
int rsapri   (const rsakp* kp, const mpnumber* c, mpnumber* m);

/*!\fn int rsapricrt(const rsakp* kp, const mpnumber* c, mpnumber* m)
 * \brief This function performs a raw RSA private key operation, with
 *  application of the Chinese Remainder Theorem.
 *
 * It performs the operation:
 * \li \f$j_1=c^{d_1}\ \textrm{mod}\ p\f$
 * \li \f$j_2=c^{d_2}\ \textrm{mod}\ q\f$
 * \li \f$h=c \cdot (j_1-j_2)\ \textrm{mod}\ p\f$
 * \li \f$m=j_2+hq\f$
 *
 * \param kp The RSA keypair.
 * \param c The ciphertext.
 * \param m The message.
 * \retval 0 on success.
 * \retval -1 on failure.
 */
BEECRYPTAPI
int rsapricrt(const rsakp* kp, const mpnumber* c, mpnumber* m);

/*!\fn int rsavrfy(const rsapk* pk, const mpnumber* m, const mpnumber* c)
 * \brief This function performs a raw RSA verification.
 *
 * It verifies if ciphertext \a c was encrypted from cleartext \a m
 * with the private key matching the given public key \a pk.
 *
 * \param pk The public key.
 * \param m The cleartext message.
 * \param c The ciphertext message.
 * \retval 1 on success.
 * \retval 0 on failure.
 */
BEECRYPTAPI
int rsavrfy  (const rsapk* pk, const mpnumber* m, const mpnumber* c);

#ifdef __cplusplus
}
#endif

#endif
