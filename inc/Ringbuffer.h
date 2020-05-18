/**
 *  \brief     Implements a ringbuffer.
 *  \details   template for ringbuffer.\n
 *             stores element of template parameter, write and read is handled by buffer.
 *
 *  \author    j. Wuebbelmann
 *  \version   1.0
 *  \date      24.06.2016
 *  \pre       none
 *  \copyright HS Osnabrueck
 */

/** @file */

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#ifdef DEBUG
#include <cstdio>
#endif


/**
 * \class Ringbuffer inc/Ringbuffer.h
 * \warning The class does not care if unread entries are overwritten or empty slots are read.\n
 * The user has to take care that accesses are sychronised!
 */
template<typename T>
class Ringbuffer
{

public:
	/*! \brief Constructor of Ringbuffer
	 *  \param[in] size  number of elements of type T to store
	 *  \param[in] emptyItem  item of type T is written in empty slots of buffer if DEBUG is enabled
	 *  \param[in] buffer     array of \ref m_N elements of type T.\n
	 *                        If buffer is nullptr, a new buffer is created with new.
	 *
	 */
	Ringbuffer(uint32_t size, T emptyItem, T* buffer = nullptr);
    /*! \brief destructor\n
     * \details if memory is in ownership of buffer, the memory is deleted.
     *
     */
	virtual ~Ringbuffer();

    /*!
     * \brief write access to ringbuffer\n
     * \details an item is written in buffer.\n
     *          \ref m_in is incremented and wrapped around if \ref m_N is reached.\n
     *          If DEBUG is defined, it is checked if \ref m_emptyItem is overwritten.
     *  \note If \ref m_emptyItem is valid element to store, DEBUG should be undefined!
     *  \param[in] item  The item of type T to write in buffer.
     *  \return    reference to ringbuffer (*this).
     */
	Ringbuffer& operator << (const T& item);

	/*!
     * \brief read access to ringbuffer\n
     * \details an item is read from buffer.\n
     *          \ref m_out is incremented and wrapped around if \ref m_N is reached.\n
     *          If DEBUG is defined, \ref m_emptyItem is written is read buffer slot.
     *  \note If \ref m_emptyItem is valid element to store, DEBUG should be undefined!
     *  \param[out] item  reference of item of type T in which to write buffer element.
     *  \return    reference to ringbuffer (*this).
     */
	Ringbuffer& operator >> (T& item);

	/*!
	 * \brief return size of ringbuffer
	 * \return \ref m_N, number of elements of type T which can be stored in buffer
	 */
    uint32_t    getSize() const {return m_N;}
private:
    uint32_t  m_N;         ///< size of buffer
    uint32_t  m_out;       ///< pointer used by consumer
    uint32_t  m_in;        ///< pointer used by producer
    T*        m_buffer;    ///< buffer for Data
    bool      m_isDynamic; ///< does Ringbuffer owns buffer memory?
	T         m_emptyItem; ///< is written into buffer after read (burn after reading)

    Ringbuffer(const Ringbuffer&) = delete;
};

/*! \cond HIDDEN_DOXYGEN*/
template<typename T>
Ringbuffer<T>::Ringbuffer(uint32_t size, T emptyItem, T* buffer):
		m_N(size),
		m_out(0),
		m_in(0),
		m_buffer(buffer),
		m_isDynamic(false),
		m_emptyItem(emptyItem)
{
    // do we need to alloc memory?
	if (!m_buffer)
	{
		m_buffer = new T[m_N];
		m_isDynamic = true;
	}
#ifdef DEBUG
	for (uint32_t i = 0; i < m_N; ++i) m_buffer[i] = m_emptyItem;
#endif
}

template<typename T>
Ringbuffer<T>::~Ringbuffer()
{
  if (m_isDynamic)
  {
	  delete [] m_buffer;
  }
}

template<typename T>
Ringbuffer<T>& Ringbuffer<T>::operator << (const T& item)
{
#ifdef DEBUG
	if (m_emptyItem != m_buffer[m_in] )
	{
		fprintf(stderr,"*** %s %d write Item is in use!\n", __FILE__, __LINE__);
	}
#endif
	m_buffer[m_in++] = item;
	m_in %= m_N;
	return *this;
}
template<typename T>
Ringbuffer<T>& Ringbuffer<T>::operator >> (T& item)
{
	item = m_buffer[m_out];
#ifdef DEBUG
	if (item == m_emptyItem)
	{
		fprintf(stderr,"*** %s %d read Item is empty!\n", __FILE__, __LINE__);
	}
	m_buffer[m_out] = m_emptyItem;
#endif
	++m_out %= m_N;
	return *this;
}
/*!
 * \endcond
 */

#endif /* INC_RINGBUFFER_H_ */
